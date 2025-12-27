#include "interactive/interactive_mode.h"
#include "interactive/query_defs.h"
#include "interactive/ui_components.h"
#include "io/command_processor.h"
#include "validation/validate_arg.h"
#include "io/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

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
        if (!q) break; 

        if (get_query_id(q) == 0) break;

        int com_S = 0;
        
        if (get_query_permite_s(q)) {
            int escolha = ui_menu_formato_resultado(get_query_id(q));
            if (escolha == 2) continue; // voltar ao menu principal
            com_S = escolha; // 0 = normal, 1 = S
        }

        int argumentos_validos = 0;
        int cancelar_query = 0;

        while (!argumentos_validos) {  
            ui_pedir_argumentos(q, com_S, args, sizeof(args));

            int tokens = contar_tokens(args);
            int obrigatorios = 0;
            
            int num_args = get_query_num_args(q);
            
            for (int i = 0; i < num_args; i++) {
                const QueryArg* arg = get_query_arg_at(q, i);
                if (get_arg_obrigatorio(arg)) obrigatorios++;
            }

            if (tokens < obrigatorios) {
                // se faltam argumentos, mostrar erro
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
                char *token = strtok(args, " ");
                while (token && n_args < 10) {
                    arg_tokens[n_args++] = token;
                    token = strtok(NULL, " ");
                }

                // validar semanticamente os argumentos
                ValidationResult* res = validar_query(get_query_id(q), arg_tokens);
                if (!validation_result_get_ok(res)) {
                    int escolha;
                    ui_mostrar_erro_arg(res, &escolha);
                    if (escolha == 0) continue; // tentar novamente
                    else {
                        cancelar_query = 1;
                        break;
                    }
                }
                free(res);     
            }
        }

        if (cancelar_query) continue;

        char linha[512];
        
        snprintf(linha, sizeof(linha), "%d%s %s", get_query_id(q), com_S ? "S" : "", args);

        def_prog_mode(); 
        endwin();
        
        if (system("clear") == -1) {} 
        
        printf("=== RESULTADOS DA QUERY %d ===\n\n", get_query_id(q));

        execute_single_line(linha, manager, 0, 1);

        printf("\n\nPrima ENTER para voltar ao menu...");
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        if (system("clear") == -1) {}
        
        reset_prog_mode();
        refresh();
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