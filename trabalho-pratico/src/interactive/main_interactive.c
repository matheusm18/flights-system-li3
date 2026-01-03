#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/parser.h"
#include "io/command_processor.h"
#include "interactive/interactive_mode.h"
#include "interactive/query_defs.h"
#include <ncurses.h>

int main(int argc, char **argv) {

    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    // inicializar ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();

        use_default_colors(); 
        
        // branco no preto (geral)
        init_pair(1, COLOR_WHITE, -1);
        // ciano no preto (titulos)
        init_pair(2, COLOR_CYAN, -1);
        // amarelo no preto (titulos)
        init_pair(3, COLOR_YELLOW, -1);
        // verde no preto (sucesso)
        init_pair(4, COLOR_GREEN, -1);
        // vermelho no preto (avisos/erro)
        init_pair(5, COLOR_RED, -1);
    }

    erase();
    refresh();

    QueryManager* qm = create_query_manager(10);
    init_all_queries(qm);

    CatalogManager* catalog_manager = catalog_manager_create();

    start_interactive_ui(catalog_manager, qm);

    endwin();

    catalog_manager_destroy(catalog_manager);
    free_query_manager(qm);
    return 0;
}
