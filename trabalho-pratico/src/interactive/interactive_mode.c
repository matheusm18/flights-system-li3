#include "interactive/interactive_mode.h"
#include "interactive/query_defs.h"
#include "interactive/ui_components.h"
#include "io/command_processor.h"
#include "validation/validate_arg.h"
#include "io/parser.h"
#include "queries/query_result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "io/output_writer.h"

static int contar_tokens(const char* str) {
    int count = 0;
    char tmp[512];
    if (!str || strlen(str) == 0) return 0;

    strcpy(tmp, str);

    char* tok = strtok(tmp, " ");
    while (tok) {
        count++;
        tok = strtok(NULL, " ");
    }
    return count;
}

void run_menu_loop(CatalogManager* manager) {
    char args[256];

    while (1) {
        bkgd(COLOR_PAIR(1));
        erase();
        refresh();

        int q_index = ui_menu_queries(7); // 6 queries + opção sair
        
        const Query* q = get_query_at_index(q_index);
        if (!q || get_query_id(q) == 0) break; 

        int com_S = 0;
        if (get_query_permite_s(q)) {
            int escolha = ui_menu_formato_resultado(get_query_id(q));
            if (escolha == 2) continue; // voltar ao menu principal
            com_S = escolha; // 0 = normal, 1 = S
        }

        int argumentos_validos = 0;
        int cancelar_query = 0;
        
        char args_copy[256]; 

        while (!argumentos_validos) {  
            ui_pedir_argumentos(q, com_S, args, sizeof(args));

            strcpy(args_copy, args); // usar args_copy para validações (strtok) e args original para executar

            int tokens = contar_tokens(args);
            int obrigatorios = 0;
            
            int num_args = get_query_num_args(q);
            for (int i = 0; i < num_args; i++) {
                const QueryArg* arg = get_query_arg_at(q, i);
                if (get_arg_obrigatorio(arg)) obrigatorios++;
            }

            if (tokens < obrigatorios) {
                int escolha = ui_menu_aviso_argumentos(obrigatorios, tokens);
                if (escolha == 0) {
                    continue; // tentar novamente
                } else {
                    cancelar_query = 1; // voltar ao menu
                    break;
                }
            } else {
                char *arg_tokens[10];
                int n_args = 0;
                char *token = strtok(args_copy, " "); 
                
                while (token && n_args < 10) {
                    arg_tokens[n_args++] = token;
                    token = strtok(NULL, " ");
                }

                // validar semanticamente os argumentos
                ValidationResult* res = validar_query(get_query_id(q), arg_tokens);
                if (!validation_result_get_ok(res)) {
                    int escolha;
                    ui_mostrar_erro_arg(res, &escolha);
                    if (escolha == 0) {
                        } 
                    else {
                        cancelar_query = 1;
                        free(res);
                        break;
                    }
                } 
                else {
                    // se passou na validação, marcamos como válido para sair do loop
                    argumentos_validos = 1;
                }
                free(res);     
            }
        }

        if (cancelar_query) continue;

        char linha[512];
        snprintf(linha, sizeof(linha), "%d%s %s", get_query_id(q), com_S ? "S" : "", args);

        QueryResult* result = execute_single_line(linha, manager, 0, 0, NULL); 

        if (result == NULL) {
            mvprintw(LINES - 1, 0, "Erro ao executar query. Prima ENTER...");
            refresh();
            flushinp();
            int c;
            while ((c = getch()) != '\n' && c != '\r' && c != KEY_ENTER);
            continue;
        }

        int total_lines = get_result_num_lines(result);

        bkgd(COLOR_PAIR(1));
        erase();
        refresh();

        int frame_height = LINES - 2;
        if (frame_height < 10) frame_height = 10;

        WINDOW* frame = newwin(frame_height, COLS, 0, 0);
        if (frame == NULL) {
            destroy_query_result(result);
            mvprintw(LINES - 1, 0, "Erro ao criar janela. Prima ENTER...");
            refresh();
            flushinp();
            int c;
            while ((c = getch()) != '\n' && c != '\r' && c != KEY_ENTER);
            continue;
        }

        wbkgd(frame, COLOR_PAIR(1));
        box(frame, 0, 0);

        wattron(frame, A_BOLD | COLOR_PAIR(2));
        mvwprintw(frame, 0, (COLS - 24) / 2, " RESULTADOS DA QUERY ");
        wattroff(frame, A_BOLD | COLOR_PAIR(2));
        wrefresh(frame);

        // criar PAD para o conteúdo (área scrollável)
        int pad_height = total_lines + 10;
        int pad_width = COLS - 4;

        WINDOW* content_pad = newpad(pad_height, pad_width);
        if (content_pad == NULL) {
            delwin(frame);
            destroy_query_result(result);
            mvprintw(LINES - 1, 0, "Erro ao criar pad. Prima ENTER...");
            refresh();
            flushinp();
            int c;
            while ((c = getch()) != '\n' && c != '\r' && c != KEY_ENTER);
            continue;
        }

        wbkgd(content_pad, COLOR_PAIR(1));
        keypad(content_pad, TRUE); // ativa o suporte a teclas especiais

        // escrever conteúdo no pad
        write_result(result, NULL, com_S ? '=' : ';', 1, content_pad);

        // calcular área visível e limites de scroll
        int visible_height = frame_height - 3;
        int max_scroll = total_lines - visible_height + 2;
        if (max_scroll < 0) max_scroll = 0;

        int scroll_pos = 0;

        // mostrar conteúdo inicial
        prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3);  // visible_height limita até onde desenha no ecra

        mvprintw(LINES - 2, 2, "Faça scroll para navegar pelos resultados | ENTER para voltar");
        refresh();

        // loop de eventos
        int ch;
        MEVENT event;

        while (1) {
            ch = wgetch(content_pad);
            
            if (ch == '\n' || ch == '\r' || ch == KEY_ENTER || ch == 'q' || ch == 'Q') {
                break;
            }
            else if (ch == KEY_MOUSE) {
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON4_PRESSED && scroll_pos > 0) {
                        scroll_pos--;
                        prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3); // scroll_pos é a linha de onde começa a visualização
                    }
                    else if (event.bstate & BUTTON5_PRESSED && scroll_pos < max_scroll) {
                        scroll_pos++;
                        prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3);
                    }
                }
            }
            else if (ch == KEY_UP && scroll_pos > 0) {
                scroll_pos--;
                prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3);
            }
            else if (ch == KEY_DOWN && scroll_pos < max_scroll) {
                scroll_pos++;
                prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3);
            }
            else if (ch == KEY_PPAGE && scroll_pos > 0) {
                scroll_pos -= (visible_height - 2);
                if (scroll_pos < 0) scroll_pos = 0;
                prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3);
            }
            else if (ch == KEY_NPAGE && scroll_pos < max_scroll) {
                scroll_pos += (visible_height - 2);
                if (scroll_pos > max_scroll) scroll_pos = max_scroll;
                prefresh(content_pad, scroll_pos, 0, 2, 2, visible_height, COLS - 3);
            }
        }

        delwin(content_pad);
        delwin(frame);
        destroy_query_result(result);

    }
}


void start_interactive_ui(CatalogManager* manager) {
    wbkgd(stdscr, COLOR_PAIR(1));
    
    ui_menu_inicial();

    char dataset_path[512];

    while (1) {
        erase(); 
        refresh();

        char* input = ui_pedir_caminho_dataset();
        
        if (strlen(input) == 0) {
            strcpy(dataset_path, "dataset-fase-2/sem_erros/");
        } else {
            strcpy(dataset_path, input);
        }

        int len = strlen(dataset_path);
        if (len > 0 && dataset_path[len-1] != '/') {
            strcat(dataset_path, "/");
        }

        GDir* dir = g_dir_open(dataset_path, 0, NULL);
        if (dir != NULL) {
            g_dir_close(dir);
            break;
        } else {
            ui_mostrar_erro_dataset();
        }
    }

    WINDOW* load_win = ui_mostrar_carregamento_inicio();

    load_datasets(dataset_path, manager);

    ui_mostrar_carregamento_fim(load_win);
    
    run_menu_loop(manager);
}