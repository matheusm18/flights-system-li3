#include "validation/validate_arg.h"
#include "interactive/ui_components.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FRAME_WIDTH   160
#define FRAME_HEIGHT   39

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
    
    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS  - FRAME_WIDTH)  / 2;

    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);
    
    WINDOW* pad = newpad(20, 75);
    wbkgd(pad, COLOR_PAIR(1));
    box(pad, 0, 0);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - 20) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH  - 75)  / 2;

    wattron(pad, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(pad, 1, 17, "_");
    mvwprintw(pad, 2, 15, "-=\\`\\");
    mvwprintw(pad, 3, 11, "|\\ ____\\_\\__");
    mvwprintw(pad, 4, 9, "-=\\c`\"\"\"\"\"\"\" \"`)");
    mvwprintw(pad, 5, 13, "~~~~~/ /~~`");
    mvwprintw(pad, 6, 14, "-==/ / ");
    mvwprintw(pad, 7, 16 , "'-'");
    mvwprintw(pad, 8, 31, "\\       /            _\\/_");
    mvwprintw(pad, 9, 33, ".-'-.              //o\\  _\\/_");
    mvwprintw(pad, 10, 14, "_  ___  __  _ --_ /     \\ _--_ __  __ _ | __/o\\\\ _");
    mvwprintw(pad, 11, 13, "=-=_=-=-_=-=_=-_= -=======- = =-=_=-=_,-'|\"'\"\"-|-,_ ");
    mvwprintw(pad, 12, 13, "=- _=-=-_=- _=-= _--=====- _=-=_-_,-\"          |");
    mvwprintw(pad, 13, 15, "=- =- =-= =- = -  -===- -= - .");
    wattroff(pad, COLOR_PAIR(2) | A_BOLD);

    int y = 16;
    int x = (75 - strlen("[pressione ENTER para indicar o caminho de dados]")) / 2;

    mvwprintw(pad, y, x + 2, "[pressione ");
    wattron(pad, COLOR_PAIR(3) | A_BOLD);  
    wprintw(pad, "ENTER");
    wattroff(pad, COLOR_PAIR(3) | A_BOLD); 
    wprintw(pad, " para indicar o caminho de dados]");

    wrefresh(frame);

    prefresh(pad,0, 0, pad_screen_y, pad_screen_x, pad_screen_y + 20 - 1, pad_screen_x + 75  - 1);

    while (wgetch(frame) != 10) {}

    delwin(pad);
    delwin(frame);
    wattrset(stdscr, A_NORMAL); 
    attrset(COLOR_PAIR(0));    
    clear();
    refresh();
    curs_set(1);
}


char* ui_pedir_caminho_dataset() {
    static char path[200];
    memset(path, 0, sizeof(path));

    int pos = 0;
    int ch;
    int input_y = 5;
    int input_x = 1 + 24;   // após "C:\> "
    int max_width  = 30;    
    int max_height = 7;     // número de linhas da "caixa do computador"
    int line = 0;

    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS  - FRAME_WIDTH)  / 2;

    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    WINDOW* pad = newpad(25, 78);
    wbkgd(pad, ' ' | A_NORMAL);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - 25) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH  - 78) / 2;


    mvwprintw(pad, 1,  2, "              _____________________________________________");
    mvwprintw(pad, 2,  2, "             /                                             \\");
    mvwprintw(pad, 3,  2, "            |    _______________________________________    |");
    mvwprintw(pad, 4,  2, "            |   |                                       |   |");
    mvwprintw(pad, 5,  2, "            |   | C:\\>                                  |   |");
    mvwprintw(pad, 6,  2, "            |   |                                       |   |");
    mvwprintw(pad, 7,  2, "            |   |                                       |   |");
    mvwprintw(pad, 8,  2, "            |   |                                       |   |");
    mvwprintw(pad, 9,  2, "            |   |                                       |   |");
    mvwprintw(pad, 10, 2, "            |   |                                       |   |");
    mvwprintw(pad, 11, 2, "            |   |                                       |   |");
    mvwprintw(pad, 12, 2, "            |   |_______________________________________|   |");
    mvwprintw(pad, 13, 2, "            |                                               |");
    mvwprintw(pad, 14, 2, "             \\_____________________________________________/");
    mvwprintw(pad, 15, 2, "                   \\___________________________________/");
    mvwprintw(pad, 16, 2, "                ___________________________________________");
    mvwprintw(pad, 17, 2, "             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_");
    mvwprintw(pad, 18, 2, "          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_");
    mvwprintw(pad, 19, 2, "       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_");
    mvwprintw(pad, 20, 2, "    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_");
    mvwprintw(pad, 21, 2, " _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_");
    mvwprintw(pad, 22, 2, ":-------------------------------------------------------------------------:");
    mvwprintw(pad, 23, 2, "`---._.-------------------------------------------------------------._.---'");

    curs_set(1);
    noecho();
    wrefresh(frame);
    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + 25 - 1,pad_screen_x + 78 - 1);

    while (1) {
        wmove(pad, input_y + line, input_x + pos);
        prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + 25 - 1, pad_screen_x + 78 - 1);

        ch = wgetch(frame);

        if (ch == '\n' || ch == KEY_ENTER)
            break;

        else if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8)) {
            if (pos > 0) {
                pos--;
                path[pos + line * max_width] = '\0';
                mvwaddch(pad, input_y + line, input_x + pos, ' ');
            } else if (line > 0) {
                line--;
                pos = max_width;
            }
        }
        else if (isprint(ch) && pos + line*max_width < 200) { // limite do buffer
            path[pos + line * max_width] = ch;
            mvwaddch(pad, input_y + line, input_x + pos, ch);
            pos++;

            if (pos >= max_width) { // ir para linha seguinte
                if (line + 1 < max_height) {
                    line++;
                    pos = 0;
                } else {
                    pos = max_width - 1; // não deixa ultrapassar a caixa
                }
            }
        }
    }

    curs_set(0);
    delwin(pad);
    delwin(frame);
    return path;
}


int ui_menu_queries(int n) {
    curs_set(0);
    int highlight = 0;
    int ch;

    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS - FRAME_WIDTH) / 2;

    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    int pad_width  = largura_menu_queries(n);        
    int pad_height = n + 5;
    if (pad_height > FRAME_HEIGHT - 2) pad_height = FRAME_HEIGHT - 2;

    WINDOW* pad = newpad(pad_height, pad_width);
    wbkgd(pad, ' ' | A_NORMAL);
    box(pad,0,0);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - pad_height) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH  - pad_width)  / 2; 

    wattron(pad, COLOR_PAIR(2) | A_BOLD);  
    mvwprintw(pad, 1, (pad_width - strlen("=== MENU DE QUERIES ==="))/2, "=== MENU DE QUERIES ===");
    wattroff(pad, COLOR_PAIR(2) | A_BOLD); 

    wrefresh(frame);  
    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

    while (1) {
        for (int i = 0; i < n; i++) {
            if (i == highlight) wattron(pad, A_REVERSE); // wattron(WINDOW *win, int attr); liga um atributo a uma janela específica
            
            const Query* q = get_query_at_index(i);
            
            /*
            menuwin - janela onde vai escrever
            i+2 - linha da janela onde o cursor se vai posicionar
            2 - coluna da janela onde o cursor se vai posicionar
            */

            if (q) {
                mvwprintw(pad, i + 3, 2, "%d - %-15s | %s", get_query_id(q), get_query_nome(q), get_query_descricao(q));
            }
            wattroff(pad, A_REVERSE); // desliga
        }

        prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

        ch = wgetch(frame);

        switch (ch) {
            case KEY_UP: 
                highlight = (highlight - 1 + n) % n; 
                break;
            case KEY_DOWN: 
                highlight = (highlight + 1) % n; 
                break;
            case 10:  // ENTER
                delwin(pad);
                delwin(frame); // apaga a janela
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
        "  - resultados separados por '='",
        "[voltar]"
    };

    int n_options = 3;
    int highlight = 0;
    int ch;

    int max_len = 0;
    for (int i = 0; i < n_options; i++) {
        int len = strlen(options[i]);
        if (len > max_len) max_len = len;
    }

    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS - FRAME_WIDTH) / 2;
    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    int pad_height = n_options + 5; 
    int pad_width = max_len + 8;   
    if (pad_width > FRAME_WIDTH - 4) pad_width = FRAME_WIDTH - 4; 

    WINDOW* pad = newpad(pad_height, pad_width);
    wbkgd(pad, ' ' | A_NORMAL);
    box(pad, 0, 0);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - pad_height) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH - pad_width) / 2;

    char buf[9];
    snprintf(buf, sizeof(buf), "QUERY %d", query_id);
    wattron(pad, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(pad, 1, (pad_width - strlen(buf)) / 2, "%s", buf);
    wattroff(pad, COLOR_PAIR(2) | A_BOLD);

    wrefresh(frame);
    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

    while (1) {
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(pad, A_REVERSE);

            if (i == 0)
                mvwprintw(pad, i + 3, 4, "%d%s", query_id, options[i]);
            else if (i == 1)
                mvwprintw(pad, i + 3, 4, "%dS%s", query_id, options[i]);
            else
                mvwprintw(pad, i + 4, (pad_width - strlen(options[i])) / 2, "%s", options[i]);

            if (i == highlight) wattroff(pad, A_REVERSE);
        }


        prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);
        ch = wgetch(frame);

        if (ch == 10) { // ENTER
            delwin(pad);
            delwin(frame);
            clear();        
            refresh();  
            curs_set(1);    
            return highlight;
        }

        if (ch == KEY_UP) highlight = (highlight - 1 + n_options) % n_options;
        if (ch == KEY_DOWN) highlight = (highlight + 1) % n_options;
    }
}


void ui_pedir_argumentos(const Query* q, int com_S, char* buffer, int size) {
    clear();
    refresh();

    int num_args = get_query_num_args(q);

    int width = FRAME_WIDTH;
    int height = FRAME_HEIGHT;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    keypad(win, TRUE);
    wbkgd(win, ' ' | A_NORMAL);
    box(win, 0, 0);

    char titulo_caixa[32];
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
    int box_width = 25; 
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

    int pad_width = 50;
    int pad_height = n_options + 7;
    
    int frame_height = FRAME_HEIGHT;
    int frame_width  = FRAME_WIDTH;
    int frame_y = (LINES - frame_height) / 2;
    int frame_x = (COLS - frame_width) / 2;
    
    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);
    wrefresh(frame);

    WINDOW* pad = newpad(pad_height, pad_width);
    wbkgd(pad, ' ' | A_NORMAL);
    box(pad, 0, 0);

    int pad_screen_y = frame_y + (frame_height - pad_height) / 2;
    int pad_screen_x = frame_x + (frame_width - pad_width) / 2;


    char titulo[] = " [!] AVISO [!] ";
    wattron(pad, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(pad, 0, (pad_width - strlen(titulo)) / 2, "%s", titulo);
    wattroff(pad, COLOR_PAIR(5) | A_BOLD);

    print_center(pad, 2, "Faltam argumentos obrigatorios.");
    char buffer[100];

    snprintf(buffer, sizeof(buffer), "Esperado pelo menos %d argumento(s).", obrigatorios);
    print_center(pad, 3, buffer);

    snprintf(buffer, sizeof(buffer),"Recebido: %d.", recebidos);
    print_center(pad, 4, buffer);

    int button_y = 6;
    int spacing = 4;

    int total_width = strlen(options[0]) + strlen(options[1]) + spacing;
    int start_button_x = (pad_width - total_width) / 2;

    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

    while (1) {
        int x = start_button_x;

        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(pad, A_REVERSE);
            mvwprintw(pad, button_y, x, "%s", options[i]);
            if (i == highlight) wattroff(pad, A_REVERSE);

            x += strlen(options[i]) + spacing;
        }
       
        wrefresh(frame);
        prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

        ch = wgetch(frame);
        switch (ch) {
            case KEY_LEFT:
                highlight = (highlight - 1 + n_options) % n_options;
                break;

            case KEY_RIGHT:
                highlight = (highlight + 1) % n_options;
               break;
            case 10: 
                delwin(pad);
                delwin(frame);
                curs_set(1);
                return highlight; 
        }
    }
}

void ui_mostrar_erro_dataset() {
    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS  - FRAME_WIDTH)  / 2;

    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    WINDOW* pad = newpad(25, 78);
    wbkgd(pad, ' ' | A_NORMAL);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - 25) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH  - 78) / 2;


    mvwprintw(pad, 1,  2, "              _____________________________________________");
    mvwprintw(pad, 2,  2, "             /                                             \\");
    mvwprintw(pad, 3,  2, "            |    _______________________________________    |");
    mvwprintw(pad, 4,  2, "            |   |                                       |   |");
    mvwprintw(pad, 5,  2, "            |   |                                       |   |");
    mvwprintw(pad, 6,  2, "            |   |                                       |   |");
    mvwprintw(pad, 7,  2, "            |   |                                       |   |");
    mvwprintw(pad, 8,  2, "            |   |                                       |   |");
    mvwprintw(pad, 9,  2, "            |   |                                       |   |");
    mvwprintw(pad, 10, 2, "            |   |                                       |   |");
    mvwprintw(pad, 11, 2, "            |   |                                       |   |");
    mvwprintw(pad, 12, 2, "            |   |_______________________________________|   |");
    mvwprintw(pad, 13, 2, "            |                                               |");
    mvwprintw(pad, 14, 2, "             \\_____________________________________________/");
    mvwprintw(pad, 15, 2, "                   \\___________________________________/");
    mvwprintw(pad, 16, 2, "                ___________________________________________");
    mvwprintw(pad, 17, 2, "             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_");
    mvwprintw(pad, 18, 2, "          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_");
    mvwprintw(pad, 19, 2, "       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_");
    mvwprintw(pad, 20, 2, "    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_");
    mvwprintw(pad, 21, 2, " _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_");
    mvwprintw(pad, 22, 2, ":-------------------------------------------------------------------------:");
    mvwprintw(pad, 23, 2, "`---._.-------------------------------------------------------------._.---'");

    wattron(pad, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(pad, 5, 20, "Caminho de dataset invalido!  ");
    wattroff(pad,COLOR_PAIR(5) | A_BOLD);

    mvwprintw(pad, 7, 20, "[pressione "); 
    wattron(pad, COLOR_PAIR(3) | A_BOLD);
    wprintw(pad, "ENTER");
    wattroff(pad, COLOR_PAIR(3) | A_BOLD);
    wprintw(pad, " para voltar]");

    wrefresh(frame);
    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + 25 - 1, pad_screen_x + 78 - 1);

    int ch;
    flushinp(); 
    while ((ch = wgetch(frame)) != 10 && ch != KEY_ENTER); 

    delwin(pad);
    delwin(frame);
}

WINDOW* ui_mostrar_carregamento_inicio() {
    clear();
    refresh();

    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS  - FRAME_WIDTH)  / 2;
    

    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    WINDOW* pad = newpad(25, 78);
    wbkgd(pad, ' ' | A_NORMAL);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - 25) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH  - 78) / 2;


    mvwprintw(pad, 1,  2, "              _____________________________________________");
    mvwprintw(pad, 2,  2, "             /                                             \\");
    mvwprintw(pad, 3,  2, "            |    _______________________________________    |");
    mvwprintw(pad, 4,  2, "            |   |                                       |   |");
    mvwprintw(pad, 5,  2, "            |   |                                       |   |");
    mvwprintw(pad, 6,  2, "            |   |                                       |   |");
    mvwprintw(pad, 7,  2, "            |   |                                       |   |");
    mvwprintw(pad, 8,  2, "            |   |                                       |   |");
    mvwprintw(pad, 9,  2, "            |   |                                       |   |");
    mvwprintw(pad, 10, 2, "            |   |                                       |   |");
    mvwprintw(pad, 11, 2, "            |   |                                       |   |");
    mvwprintw(pad, 12, 2, "            |   |_______________________________________|   |");
    mvwprintw(pad, 13, 2, "            |                                               |");
    mvwprintw(pad, 14, 2, "             \\_____________________________________________/");
    mvwprintw(pad, 15, 2, "                   \\___________________________________/");
    mvwprintw(pad, 16, 2, "                ___________________________________________");
    mvwprintw(pad, 17, 2, "             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_");
    mvwprintw(pad, 18, 2, "          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_");
    mvwprintw(pad, 19, 2, "       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_");
    mvwprintw(pad, 20, 2, "    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_");
    mvwprintw(pad, 21, 2, " _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_");
    mvwprintw(pad, 22, 2, ":-------------------------------------------------------------------------:");
    mvwprintw(pad, 23, 2, "`---._.-------------------------------------------------------------._.---'");

    wattron(pad, A_BOLD);
    mvwprintw(pad, 5, 20, "> A carregar dados...");
    wattroff(pad, A_BOLD);

    wrefresh(frame);
    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + 25 - 1, pad_screen_x + 78 - 1);
    
    return frame;
}

void ui_mostrar_carregamento_fim() {
    curs_set(0);
    noecho();

    int frame_y = (LINES - FRAME_HEIGHT) / 2;
    int frame_x = (COLS  - FRAME_WIDTH)  / 2;

    WINDOW* frame = newwin(FRAME_HEIGHT, FRAME_WIDTH, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    WINDOW* pad = newpad(25, 78);
    wbkgd(pad, ' ' | A_NORMAL);

    int pad_screen_y = frame_y + (FRAME_HEIGHT - 25) / 2;
    int pad_screen_x = frame_x + (FRAME_WIDTH  - 78) / 2;


    mvwprintw(pad, 1,  2, "              _____________________________________________");
    mvwprintw(pad, 2,  2, "             /                                             \\");
    mvwprintw(pad, 3,  2, "            |    _______________________________________    |");
    mvwprintw(pad, 4,  2, "            |   |                                       |   |");
    mvwprintw(pad, 5,  2, "            |   |                                       |   |");
    mvwprintw(pad, 6,  2, "            |   |                                       |   |");
    mvwprintw(pad, 7,  2, "            |   |                                       |   |");
    mvwprintw(pad, 8,  2, "            |   |                                       |   |");
    mvwprintw(pad, 9,  2, "            |   |                                       |   |");
    mvwprintw(pad, 10, 2, "            |   |                                       |   |");
    mvwprintw(pad, 11, 2, "            |   |                                       |   |");
    mvwprintw(pad, 12, 2, "            |   |_______________________________________|   |");
    mvwprintw(pad, 13, 2, "            |                                               |");
    mvwprintw(pad, 14, 2, "             \\_____________________________________________/");
    mvwprintw(pad, 15, 2, "                   \\___________________________________/");
    mvwprintw(pad, 16, 2, "                ___________________________________________");
    mvwprintw(pad, 17, 2, "             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_");
    mvwprintw(pad, 18, 2, "          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_");
    mvwprintw(pad, 19, 2, "       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_");
    mvwprintw(pad, 20, 2, "    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_");
    mvwprintw(pad, 21, 2, " _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_");
    mvwprintw(pad, 22, 2, ":-------------------------------------------------------------------------:");
    mvwprintw(pad, 23, 2, "`---._.-------------------------------------------------------------._.---'");

    wattron(pad, A_BOLD);
    mvwprintw(pad, 5, 20, "Carregamento concluído! ");
    wattroff(pad, A_BOLD);

    mvwprintw(pad, 7, 20, "[pressione "); 
    wattron(pad, COLOR_PAIR(3) | A_BOLD);
    wprintw(pad, "ENTER");
    wattroff(pad, COLOR_PAIR(3) | A_BOLD);
    wprintw(pad, " para continuar]");

    wrefresh(frame);
    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + 25 - 1, pad_screen_x + 78 - 1);


    int ch;
    flushinp();
    while ((ch = wgetch(frame)) != '\n' && ch != KEY_ENTER);

    delwin(pad);
    delwin(frame);
    clear();
    refresh();
    curs_set(1);
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

    int frame_height = FRAME_HEIGHT;
    int frame_width  = FRAME_WIDTH;
    int frame_y = (LINES - frame_height) / 2;
    int frame_x = (COLS - frame_width) / 2;

    WINDOW* frame = newwin(frame_height, frame_width, frame_y, frame_x);
    wbkgd(frame, ' ' | A_NORMAL);
    keypad(frame, TRUE);

    int pad_width = strlen(msg) + 20;
    if (pad_width > FRAME_WIDTH - 4) pad_width = FRAME_WIDTH - 4;

    int pad_height = 8;

    WINDOW* pad = newpad(pad_height, pad_width);
    wbkgd(pad, ' ' | A_NORMAL);
    box(pad, 0, 0);

    int pad_screen_y = frame_y + (frame_height - pad_height) / 2;
    int pad_screen_x = frame_x + (frame_width - pad_width) / 2;

    wattron(pad, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(pad, 0, (pad_width - 12)/2, "[! ERRO !]");
    wattroff(pad, COLOR_PAIR(5) | A_BOLD);

    print_center(pad, 2, msg);

    int button_y = 5;
    int spacing = 4;
    int total_width = strlen(options[0]) + strlen(options[1]) + spacing;
    int start_button_x = (pad_width - total_width) / 2;

    prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

    while (1) {
        int x = start_button_x;
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(pad, A_REVERSE);
            mvwprintw(pad, button_y, x, "%s", options[i]);
            if (i == highlight) wattroff(pad, A_REVERSE);
            x += strlen(options[i]) + spacing;
        }

        wrefresh(frame);
        prefresh(pad, 0, 0, pad_screen_y, pad_screen_x, pad_screen_y + pad_height - 1, pad_screen_x + pad_width - 1);

        ch = wgetch(frame);

        switch (ch) {
            case KEY_LEFT:
                highlight = (highlight - 1 + n_options) % n_options;
                break;
            case KEY_RIGHT:
                highlight = (highlight + 1) % n_options;
                break;
            case 10: // ENTER
                *escolha = highlight;
                delwin(pad);
                delwin(frame);
                curs_set(1);
                return;
        }
    }
}
