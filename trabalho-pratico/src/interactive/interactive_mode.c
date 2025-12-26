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
                return highlight; // retorna o índice da opção selecionada
        }
    }
}

int menu_formato_resultado(int query_id) {
    const char* options[] = {
        "Normal   (x)  - resultados separados por ';'",
        "Especial (xS) - resultados separados por '='"
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
    mvwprintw(win, 2, (width - strlen ("Escolha o formato dos seus resultados")) / 2, "Escolha o formato dos seus resultados");

    while (1) {
        for (int i = 0; i < 2; i++) {
            if (i == highlight) wattron(win, A_REVERSE);
            mvwprintw(win, i + 4, 4, "%s", options[i]);
            wattroff(win, A_REVERSE);
        }
        wrefresh(win);
        ch = wgetch(win);
        switch (ch) {
            case KEY_UP: case KEY_DOWN: highlight = 1 - highlight; break;
            case 10: delwin(win); return highlight; // 0 = normal, 1 = com S
        }
    }
}


void pedir_argumentos(Query q, int com_S, char* buffer, int size) {
    clear();
    mvprintw(2, 2, "Query selecionada: %d%s", q.id, com_S ? "S" : "");
    mvprintw(4, 2, "%s", q.descricao);

    mvprintw(6, 2, "Os argumentos indicados com [!] são obrigatórios:");
    int y = 7;
    for (int i = 0; i < q.num_args; i++) {
        mvprintw(y++, 4, "- %s %s", q.args[i].nome, q.args[i].obrigatorio ? "[!]" : "");
    }

    mvprintw(y + 1, 2, "[!] Introduza os argumentos numa linha e seperados por espaço [!]");
    mvprintw(y + 3, 2, "> ");
    refresh();

    echo();
    getnstr(buffer, size - 1);
    noecho();
}


void start_interactive_ui(CatalogManager* manager) {
    char dataset_path[512];

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));

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

    endwin();
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

        pedir_argumentos(q, com_S, args, sizeof(args));

        // validação de argumentos obrigatórios
        int tokens = contar_tokens(args);
        int obrigatorios = 0;
        for (int i = 0; i < q.num_args; i++)
            if (q.args[i].obrigatorio) obrigatorios++;
        if (tokens < obrigatorios) {
            clear();
            mvprintw(2, 2, "[!] Faltam argumentos obrigatórios.");
            mvprintw(3, 2, "[!] Esperado pelo menos %d argumento(s).", obrigatorios);
            mvprintw(4, 2, "[!] Recebido: %d.", tokens);
            mvprintw(8, 2, "Prima qualquer tecla para voltar ao menu...");
            refresh();
            getch();
            continue;
        }

        char linha[512];
        snprintf(linha, sizeof(linha), "%d%s %s", q.id, com_S ? "S" : "",args);


        clear();
        refresh();

        execute_single_line(linha, manager, 0, 1);

        mvprintw(LINES - 2, 2, "Prima qualquer tecla para voltar ao menu...");
        refresh();
        getch();
    }
}


