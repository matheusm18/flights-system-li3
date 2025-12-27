#include "interactive/interactive_mode.h"
#include "io/command_processor.h"
#include "io/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

typedef struct query_arg {
    const char* nome;
    int obrigatorio;   // 1 = sim, 0 = opcional
} QueryArg;

typedef struct Query {
    int id;
    const char* nome;
    const char* descricao;
    QueryArg* args;
    int num_args;
    int permite_S; // 1 = sim, 0 = opcional
} Query;


QueryArg q1_args[] = {
    {"ID_AEROPORTO", 1}
};

QueryArg q2_args[] = {
    {"N", 1},
    {"MANUFACTURER", 0}
};

QueryArg q3_args[] = {
    {"DATA_INICIO", 1},
    {"DATA_FIM", 1}
};

Query queries[] = {
    {1, "Query 1", "Resumo de um aeroporto", q1_args, sizeof(q1_args)/sizeof(QueryArg), 1},
    {2, "Query 2", "Top N aeronaves com mais voos", q2_args, sizeof(q2_args)/sizeof(QueryArg), 1},
    {3, "Query 3", "Aeroporto com mais voos entre duas datas", q3_args, sizeof(q3_args)/sizeof(QueryArg), 1},
    {0, "Sair", "Terminar programa", NULL, 0, 0}
};

int NUM_QUERIES = sizeof(queries)/sizeof(Query);


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

// calcula a largura do menu automaticamente
int largura_menu_queries(Query* queries, int n) {
    int max = 0;
    char buffer[512];
    for (int i = 0; i < n; i++) {
        snprintf(buffer, sizeof(buffer),"%d - %-15s | %s", queries[i].id, queries[i].nome,queries[i].descricao);
        int len = strlen(buffer);
        if (len > max)
            max = len;
    }
    return max + 6; // margem
}

int menu_queries(Query* queries, int n) {
    curs_set(0);
    int highlight = 0;
    int ch;

    int height = n + 5;
    int width = largura_menu_queries(queries, n);
    if (width > COLS - 4) width = COLS - 4;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* menuwin = newwin(height, width, starty, startx);
    box(menuwin, 0, 0);
    keypad(menuwin, TRUE);

    mvwprintw(menuwin, 1, (width - strlen("=== MENU DE QUERIES ==="))/2, "=== MENU DE QUERIES ===");

    while (1) {
        for (int i = 0; i < n; i++) {
            if (i == highlight) wattron(menuwin, A_REVERSE); // wattron(WINDOW *win, int attr); liga um atributo a uma janela específica
            /*
            menuwin - janela onde vai escrever
            i+2 - linha da janela onde o cursor se vai posicionar
            2 - coluna da janela onde o cursor se vai posicionar
            */
            mvwprintw(menuwin, i + 3, 2, "%d - %-15s | %s", queries[i].id, queries[i].nome, queries[i].descricao);
            wattroff(menuwin, A_REVERSE); // desliga
        }
        wrefresh(menuwin); // atualiza o terminal físico com o conteúdo da janela virtual win
        ch = wgetch(menuwin); // lê uma tecla pressionada pelo utilizador

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

int menu_formato_resultado(int query_id) {
    curs_set(0);
    const char* options[] = {
        "x  - resultados separados por ';'",
        "xS - resultados separados por '='"
    };

    int highlight = 0;
    int ch;

    int max = 0;
    for (int i = 0; i < 2; i++) {
        int len = strlen(options[i]);
        if (len > max) max = len;
    }

    int height = 7;
    int width = max + 8;
    if (width > COLS - 4) width = COLS - 4;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    char buf[64];
    snprintf(buf, sizeof(buf), "QUERY %d", query_id);

    mvwprintw(win, 1, (width- strlen(buf)) / 2, "QUERY %d", query_id);

    while (1) {
        for (int i = 0; i < 2; i++) {
            if (i == highlight) wattron(win, A_REVERSE);

            if (i == 0)
                mvwprintw(win, i + 3, 4, "%d  - resultados separados por ';'", query_id);
            else
                mvwprintw(win, i + 3, 4, "%dS - resultados separados por '='", query_id);

            if (i == highlight) wattroff(win, A_REVERSE);
        }

        mvwprintw(win, 6, 4, " ");

        wrefresh(win);
        ch = wgetch(win);
        if (ch == 10) {
            delwin(win);
            clear();        
            refresh();  
            curs_set(1);    
            return highlight;
        }
        if (ch == KEY_UP || ch == KEY_DOWN)
            highlight = 1 - highlight;
    }
}


void pedir_argumentos(Query q, int com_S, char* buffer, int size) {
    clear();
    refresh();

    // calcular largura máxima da caixa principal
    int max_len = strlen("== EXECUTE QUERY xx == ");
    for (int i = 0; i < q.num_args; i++) {
        int len = strlen(q.args[i].nome) + 3; // <> ou []
        if (len > max_len) max_len = len;
    }
    int width = max_len + 17;
    int height = 12;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    char titulo_caixa[256];
    snprintf(titulo_caixa, sizeof(titulo_caixa), "== EXECUTE QUERY %d%s ==", q.id, com_S ? "S" : "");
    mvwprintw(win, 1, (width - strlen(titulo_caixa)) / 2, "%s", titulo_caixa);

    int y = 3;

    wmove(win, y++, 3);
    for (int i = 0; i < q.num_args; i++) {
        if (q.args[i].obrigatorio)
            wprintw(win, "<%s>", q.args[i].nome);
        else
            wprintw(win, "[%s]", q.args[i].nome);
        if (i < q.num_args - 1)
            wprintw(win, " ");
    }

    y++;

    // criar subjanela para legenda 
    int box_height = 4;
    int box_width = width - 6; 
    int box_starty = y;
    int box_startx = 3;

    WINDOW* legend_win = derwin(win, box_height, box_width, box_starty, box_startx);
    box(legend_win, 0, 0);

    mvwprintw(legend_win, 1, 2, "* <arg> obrigatório");
    mvwprintw(legend_win, 2, 2, "* [arg] opcional");
    wrefresh(legend_win);

    y += box_height + 1;

    // Prompt
    mvwprintw(win, y++, 2, "> ");
    wmove(win, y-1, 4); // cursor após >

    wrefresh(win);

    echo();
    wgetnstr(win, buffer, size - 1);
    noecho();

    delwin(legend_win);
    delwin(win);
}


void menu_inicial() {
    curs_set(0);
    int width = 75;   
    int height = 20;  

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);


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


    int y = 16;
    int x = (width - strlen("[pressione ENTER para continuar]")) / 2;

    mvwprintw(win, y, x, "[pressione ");
    wattron(win, COLOR_PAIR(1) | A_BOLD);  
    wprintw(win, "ENTER");
    wattroff(win, COLOR_PAIR(1) | A_BOLD); 
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

int menu_aviso_argumentos(int obrigatorios, int recebidos) {
    clear();
    refresh();
    curs_set(0);
    const char* options[] = {
        "voltar",
        "sair"
    };
    int n_options = 2;
    int highlight = 0;
    int ch;

    int width = 50;
    int height = n_options + 7;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    char titulo[] = "== [!] AVISO [!] ==";
    mvwprintw(win, 0, (width - strlen(titulo)) / 2, "%s", titulo);
    mvwprintw(win, 2, 2, "Faltam argumentos obrigatórios.");
    mvwprintw(win, 3, 2, "Esperado pelo menos %d argumento(s).", obrigatorios);
    mvwprintw(win, 4, 2, "Recebido: %d.", recebidos);

    while (1) {
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) wattron(win, A_REVERSE);
            mvwprintw(win, i + 6, 4, "%s", options[i]);
            if (i == highlight) wattroff(win, A_REVERSE);
        }
        wrefresh(win);

        ch = wgetch(win);
        switch (ch) {
            case KEY_UP: highlight = (highlight - 1 + n_options) % n_options; break;
            case KEY_DOWN: highlight = (highlight + 1) % n_options; break;
            case 10: 
                delwin(win);
                curs_set(1);
                return highlight; // 0 = voltar, 1 = sair
        }
    }
}



void start_interactive_ui(CatalogManager* manager) {
    char dataset_path[512];

    menu_inicial();

    clear();
    mvprintw(2, 2, "Introduza o caminho dos ficheiros de dados: ");
    refresh();

    echo();
    getnstr(dataset_path, sizeof(dataset_path) - 1);
    noecho();

    if (strlen(dataset_path) == 0)
        strcpy(dataset_path, "dataset-fase-2/sem_erros/");

    clear();
    mvprintw(2, 2, "A carregar dados de '%s'...", dataset_path);
    refresh();

    load_datasets(dataset_path, manager);

    mvprintw(4, 2, "Dataset carregado com sucesso!");
    mvprintw(6, 2, "Prima qualquer tecla para continuar...");
    refresh();
    getch();

    run_menu_loop(manager);
}


void run_menu_loop(CatalogManager* manager) {
    char args[256];

    while (1) {
        clear();
        refresh();

        int q_index = menu_queries(queries, NUM_QUERIES);
        Query q = queries[q_index];

        if (q.id == 0)
            break;

        int com_S = 0;
        if (q.permite_S) {
            com_S = menu_formato_resultado(q.id);
        }

        int argumentos_validos = 0;

        int escolha;
        while (!argumentos_validos) {  
            pedir_argumentos(q, com_S, args, sizeof(args));

            int tokens = contar_tokens(args);
            int obrigatorios = 0;
            for (int i = 0; i < q.num_args; i++)
                if (q.args[i].obrigatorio) obrigatorios++;

            if (tokens < obrigatorios) {
                escolha = menu_aviso_argumentos(obrigatorios, tokens);
                if (escolha == 0) {
                    continue;
                } else {
                    endwin();
                    exit(0);
                }
            } else {
                argumentos_validos = 1;
            }
        }

        char linha[512];
        snprintf(linha, sizeof(linha), "%d%s %s", q.id, com_S ? "S" : "", args);

        clear();
        refresh();

        execute_single_line(linha, manager, 0, 1);

        mvprintw(LINES - 2, 2, "Prima qualquer tecla para voltar ao menu...");
        refresh();
        getch();
    }
}
