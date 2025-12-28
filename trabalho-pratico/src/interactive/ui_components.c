#include "validation/validate_arg.h"
#include "interactive/ui_components.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// calcula a largura do menu automaticamente
int largura_menu_queries(int n) {
    int max = 0;
    char buffer[512];
    for (int i = 0; i < n; i++) {
        const Query* q = get_query_at_index(i);
        
        if (!q) continue;

        snprintf(buffer, sizeof(buffer), "%d - %-15s | %s", get_query_id(q), get_query_nome(q), get_query_descricao(q));
                 
        int len = strlen(buffer);
        if (len > max)
            max = len;
    }
    return max + 6; // margem
}

void print_center(WINDOW *w, int y, const char *text) {
      int x = (getmaxx(w) - strlen(text)) / 2;
      mvwprintw(w, y, x, "%s", text);
}

void ui_menu_inicial() {
    curs_set(0);
    noecho();
    int width = 75;   
    int height = 20;  

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    wbkgd(win, ' ' | A_NORMAL);
    keypad(win, TRUE);
    box(win, 0, 0);

    wattron(win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(win, 1, 17, "_");
    mvwprintw(win, 2, 15, "-=\\`\\");
    mvwprintw(win, 3, 11, "|\\ ____\\_\\__");
    mvwprintw(win, 4, 9, "-=\\c`\"\"\"\"\"\"\" \"`)");
    mvwprintw(win, 5, 13, "~~~~~/ /~~`");
    mvwprintw(win, 6, 14, "-==/ / ");
    mvwprintw(win, 7, 16 , "'-'");
    mvwprintw(win, 8, 31, "\\       /            _\\/_");
    mvwprintw(win, 9, 33, ".-'-.              //o\\  _\\/_");
    mvwprintw(win, 10, 14, "_  ___  __  _ --_ /     \\ _--_ __  __ _ | __/o\\\\ _");
    mvwprintw(win, 11, 13, "=-=_=-=-_=-=_=-_= -=======- = =-=_=-=_,-'|\"'\"\"-|-,_ ");
    mvwprintw(win, 12, 13, "=- _=-=-_=- _=-= _--=====- _=-=_-_,-\"          |");
    mvwprintw(win, 13, 15, "=- =- =-= =- = -  -===- -= - .");
    wattroff(win, COLOR_PAIR(2) | A_BOLD);

    int y = 16;
    int x = (width - strlen("[pressione ENTER para continuar]")) / 2;

    mvwprintw(win, y, x, "[pressione ");
    wattron(win, COLOR_PAIR(3) | A_BOLD);  
    wprintw(win, "ENTER");
    wattroff(win, COLOR_PAIR(3) | A_BOLD); 
    wprintw(win, " para continuar]");

    wrefresh(win);

    while (wgetch(win) != 10) {}

    delwin(win);
    wattrset(stdscr, A_NORMAL); 
    attrset(COLOR_PAIR(0));    
    clear();
    refresh();
    curs_set(1);
}

char* ui_pedir_caminho_dataset() {
    static char path[46]; 
    memset(path, 0, sizeof(path));
    int pos = 0;
    int ch;

    int width = 76, height = 9;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    erase();
    refresh();

    WINDOW* win = newwin(height, width, starty, startx);
    wbkgd(win, ' ' | A_NORMAL);

    keypad(win, TRUE);
    box(win, 0, 0);

    wattron(win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(win, 1, (width - 18)/2, "== LOAD DATASET ==");
    wattroff(win, COLOR_PAIR(2) | A_BOLD);
    
    mvwprintw(win, 4, 3, "Insira o caminho do dataset [vazio para o caminho padrão]");
    mvwhline(win, 6, 1, ACS_HLINE, width - 2);
    mvwprintw(win, 7, 4, "> ");
    
    curs_set(1);
    noecho();

    while (1) {
        wmove(win, 7, 6 + pos);
        wrefresh(win);
        ch = wgetch(win);

        if (ch == 10 || ch == KEY_ENTER) break;

        else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (pos > 0) {
                pos--;
                path[pos] = '\0';
                mvwaddch(win, 7, 6 + pos, ' ');
            }
        }
        // filtro: só aceita se for caractere imprimível (letras, numeros, etc)
        else if (pos < 45 && isprint(ch)) {
            path[pos] = ch;
            mvwaddch(win, 7, 6 + pos, ch);
            pos++;
        }
    }

    path[pos] = '\0';
    curs_set(0);
    delwin(win);
    return path;
}

int ui_menu_queries(int n) {
    
    curs_set(0);
    int highlight = 0;
    int ch;

    int height = n + 5;
    int width = largura_menu_queries(n);
    if (width > COLS - 4) width = COLS - 4;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* menuwin = newwin(height, width, starty, startx);
    wbkgd(menuwin, ' ' | A_NORMAL);

    box(menuwin, 0, 0);
    keypad(menuwin, TRUE);

    wattron(menuwin, COLOR_PAIR(2) | A_BOLD);  
    mvwprintw(menuwin, 1, (width - strlen("=== MENU DE QUERIES ==="))/2, "=== MENU DE QUERIES ===");
    wattroff(menuwin, COLOR_PAIR(2) | A_BOLD); 

    while (1) {
        for (int i = 0; i < n; i++) {
            if (i == highlight) wattron(menuwin, A_REVERSE); // wattron(WINDOW *win, int attr); liga um atributo a uma janela específica
            
            const Query* q = get_query_at_index(i);
            
            /*
            menuwin - janela onde vai escrever
            i+2 - linha da janela onde o cursor se vai posicionar
            2 - coluna da janela onde o cursor se vai posicionar
            */

            if (q) {
                mvwprintw(menuwin, i + 3, 2, "%d - %-15s | %s", get_query_id(q), get_query_nome(q), get_query_descricao(q));
            }
            wattroff(menuwin, A_REVERSE); // desliga
        }
        wrefresh(menuwin);
        ch = wgetch(menuwin);

        switch (ch) {
            case KEY_UP: 
                highlight = (highlight - 1 + n) % n; 
                break;
            case KEY_DOWN: 
                highlight = (highlight + 1) % n; 
                break;
            case 10:  // ENTER
                delwin(menuwin); // apaga a janela
                clear();   
                refresh();
                curs_set(1); 
                return highlight; // retorna o índice da opção selecionada
        }
    }
}

int ui_menu_formato_resultado(int query_id) {
    curs_set(0);
    const char* options[] = {
        "  - resultados separados por ';'",
        " - resultados separados por '='",
        "[voltar]"
    };

    int n_options = 3;
    int highlight = 0;
    int ch;

    int max = 0;
    for (int i = 0; i < n_options; i++) {
        int len = strlen(options[i]);
        if (len > max) max = len;
    }

    int height = 8;
    int width = max + 8;
    if (width > COLS - 4) width = COLS - 4;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    wbkgd(win, ' ' | A_NORMAL);
    box(win, 0, 0);
    keypad(win, TRUE);

    char buf[64];
    snprintf(buf, sizeof(buf), "QUERY %d", query_id);

    wattron(win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(win, 1, (width- strlen(buf)) / 2, "QUERY %d", query_id);
    wattroff(win, COLOR_PAIR(2) | A_BOLD);

    while (1) {
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(win, A_REVERSE);

            if (i == 0)
                mvwprintw(win, i + 3, 4, "%d%s", query_id, options[i]);
            else if (i == 1)
                mvwprintw(win, i + 3, 4, "%dS%s", query_id, options[i]);
            else
                mvwprintw(win, i + 4, 16, "%s", options[i]);

            if (i == highlight) wattroff(win, A_REVERSE);
        }

        wrefresh(win);
        ch = wgetch(win);

        if (ch == 10) { // enter
            delwin(win);
            clear();        
            refresh();  
            curs_set(1);    
            return highlight;
        }

        if (ch == KEY_UP) {
            highlight = (highlight - 1 + n_options) % n_options;
        } else if (ch == KEY_DOWN) {
            highlight = (highlight + 1) % n_options;
        }
    }
}

void ui_pedir_argumentos(const Query* q, int com_S, char* buffer, int size) {
    clear();
    refresh();

    int num_args = get_query_num_args(q);
    
    // calcular largura máxima da caixa principal
    int max_len = strlen("== EXECUTE QUERY xx == ");
    
    for (int i = 0; i < num_args; i++) {
        const QueryArg* arg = get_query_arg_at(q, i);
        int len = strlen(get_arg_nome(arg)) + 3; // +3 para os chars < > ou [ ]
        if (len > max_len) max_len = len;
    }
    
    int width = max_len + 17;
    int height = 12;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    keypad(win, TRUE);
    wbkgd(win, ' ' | A_NORMAL);
    box(win, 0, 0);

    char titulo_caixa[256];
    snprintf(titulo_caixa, sizeof(titulo_caixa), "== EXECUTE QUERY %d%s ==", get_query_id(q), com_S ? "S" : "");

    wattron(win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(win, 1, (width - strlen(titulo_caixa)) / 2, "%s", titulo_caixa);
    wattroff(win, COLOR_PAIR(2) | A_BOLD);

    int y = 3;

    wmove(win, y++, 3);
    for (int i = 0; i < num_args; i++) {
        const QueryArg* arg = get_query_arg_at(q, i);
        
        if (get_arg_obrigatorio(arg))
            wprintw(win, "<%s>", get_arg_nome(arg));
        else
            wprintw(win, "[%s]", get_arg_nome(arg));
            
        if (i < num_args - 1)
            wprintw(win, " ");
    }

    y++;

    // criar subjanela para legenda 
    int box_height = 4;
    int box_width = width - 6; 
    int box_starty = y;
    int box_startx = 3;

    WINDOW* legend_win = derwin(win, box_height, box_width, box_starty, box_startx);
    wbkgd(legend_win, ' ' | A_NORMAL);
    box(legend_win, 0, 0);

    mvwprintw(legend_win, 1, 2, "* <arg> obrigatório");
    mvwprintw(legend_win, 2, 2, "* [arg] opcional");
    wrefresh(legend_win);

    y += box_height + 1;

    // Prompt
    mvwprintw(win, y++, 2, "> ");
    wmove(win, y-1, 4);  // cursor após >

    wrefresh(win);

    echo();
    wgetnstr(win, buffer, size - 1);
    noecho();

    delwin(legend_win);
    delwin(win);
}

int ui_menu_aviso_argumentos(int obrigatorios, int recebidos) {
    clear();
    refresh();
    curs_set(0);
    const char* options[] = {
        "[tentar novamente]",
        "[voltar ao menu]"
    };
    int n_options = 2;
    int highlight = 0;
    int ch;

    int width = 50;
    int height = n_options + 7;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    wbkgd(win, ' ' | A_NORMAL);
    box(win, 0, 0);
    keypad(win, TRUE);

    char titulo[] = " [!] AVISO [!] ";
    wattron(win, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(win, 0, (width - strlen(titulo)) / 2, "%s", titulo);
    wattroff(win, COLOR_PAIR(5) | A_BOLD);

    print_center(win, 2, "Faltam argumentos obrigatorios.");
    char buffer[100];

    snprintf(buffer, sizeof(buffer), "Esperado pelo menos %d argumento(s).", obrigatorios);
    print_center(win, 3, buffer);

    snprintf(buffer, sizeof(buffer),"Recebido: %d.", recebidos);
    print_center(win, 4, buffer);

    int button_y = 6;
    int spacing = 4;

    int total_width = strlen(options[0]) + strlen(options[1]) + spacing;
    int start_button_x = (width - total_width) / 2;

    while (1) {
        int x = start_button_x;

        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(win, A_REVERSE);
            mvwprintw(win, button_y, x, "%s", options[i]);
            if (i == highlight) wattroff(win, A_REVERSE);

            x += strlen(options[i]) + spacing;
        }
        wrefresh(win);

        ch = wgetch(win);
        switch (ch) {
            case KEY_LEFT:
                highlight = (highlight - 1 + n_options) % n_options;
                break;

            case KEY_RIGHT:
                highlight = (highlight + 1) % n_options;
               break;
            case 10: 
                delwin(win);
                curs_set(1);
                return highlight; 
        }
    }
}

void ui_mostrar_erro_dataset() {
    erase(); 
    refresh(); 

    int width = 50, height = 8;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* err_win = newwin(height, width, starty, startx);
    wbkgd(err_win, ' ' | A_NORMAL);
    box(err_win, 0, 0);

    char* msg1 = "Caminho de dataset invalido!";
    char* msg3 = "[pressione ENTER para tentar novamente]";

    wattron(err_win, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(err_win, 3, (width - strlen(msg1)) / 2, "%s", msg1);
    wattroff(err_win, COLOR_PAIR(5) | A_BOLD);

    mvwprintw(err_win, 5, (width - strlen(msg3)) / 2, "[pressione ");
    wattron(err_win, COLOR_PAIR(3) | A_BOLD);  
    wprintw(err_win, "ENTER");
    wattroff(err_win, COLOR_PAIR(3) | A_BOLD); 
    wprintw(err_win, " para tentar novamente]");

    wrefresh(err_win);

    int ch;
    flushinp(); 
    while ((ch = wgetch(err_win)) != 10 && ch != KEY_ENTER); 

    delwin(err_win);
}

WINDOW* ui_mostrar_carregamento_inicio() {
    clear(); 
    refresh();
    
    int width = 50, height = 8;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;
    
    WINDOW* load_win = newwin(height, width, starty, startx);
    wbkgd(load_win, ' ' | A_NORMAL);
    box(load_win, 0, 0);
    keypad(load_win, TRUE);
    
    wattron(load_win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(load_win, 1, (width - 22)/2, "=== PROCESSAMENTO ===");
    wattroff(load_win, COLOR_PAIR(2) | A_BOLD);

    mvwprintw(load_win, 4, (width - 19)/2, "A carregar dados...");
    wrefresh(load_win);
    
    return load_win;
}

void ui_mostrar_carregamento_fim(WINDOW* load_win) {
    int width = 50; 
    
    wclear(load_win);
    wbkgd(load_win, ' ' | A_NORMAL);
    box(load_win, 0, 0);

    wattron(load_win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(load_win, 1, (width - 22)/2, "=== PROCESSAMENTO ===");
    wattroff(load_win, COLOR_PAIR(2) | A_BOLD);

    wattron(load_win, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(load_win, 4, (width - 23)/2, "Carregamento concluido!");
    wattroff(load_win, COLOR_PAIR(4) | A_BOLD);

    mvwprintw(load_win, 5, (width - 32)/2, "[pressione ");
    wattron(load_win, COLOR_PAIR(3) | A_BOLD);  
    wprintw(load_win, "ENTER");
    wattroff(load_win, COLOR_PAIR(3) | A_BOLD); 
    wprintw(load_win, " para continuar]");

    wrefresh(load_win);

    int ch;
    flushinp(); 
    while ((ch = wgetch(load_win)) != 10 && ch != KEY_ENTER);

    delwin(load_win);
}

void ui_mostrar_erro_arg(ValidationResult *res, int *escolha) {
    curs_set(0);
    clear();
    refresh();
    const char* msg = validation_result_get_erro(res);
    const char* options[] = {"[tentar novamente]", "[voltar ao menu]"};
    int n_options = 2;
    int highlight = 0;
    int ch;

    int width = strlen(msg) + 14;
    int height = 8;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    wbkgd(win, ' ' | A_NORMAL);
    box(win, 0, 0);
    keypad(win, TRUE);

    wattron(win, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(win, 0, (width - 12)/2, "[! ERRO !]");
    wattroff(win, COLOR_PAIR(5) | A_BOLD);

    print_center(win, 2, msg);

    int button_y = 5;
    int spacing = 4;
    int total_width = strlen(options[0]) + strlen(options[1]) + spacing;
    int start_button_x = (width - total_width) / 2;

    while (1) {
        int x = start_button_x;
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(win, A_REVERSE);
            mvwprintw(win, button_y, x, "%s", options[i]);
            if (i == highlight) wattroff(win, A_REVERSE);
            x += strlen(options[i]) + spacing;
        }

        wrefresh(win);
        ch = wgetch(win);

        switch (ch) {
            case KEY_LEFT:
                highlight = (highlight - 1 + n_options) % n_options;
                break;
            case KEY_RIGHT:
                highlight = (highlight + 1) % n_options;
                break;
            case 10: // ENTER
                *escolha = highlight;
                delwin(win);
                curs_set(1);
                return;
        }
    }
}
