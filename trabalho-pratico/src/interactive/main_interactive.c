#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/parser.h"
#include "io/command_processor.h"
#include "interactive/interactive_mode.h"
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
        
        // branco no preto (geral)
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        // ciano no preto (titulos)
        init_pair(2, COLOR_CYAN, COLOR_BLACK);
        // amarelo no preto (titulos)
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        // verde no preto (sucesso)
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        // vermelho no preto (avisos/erro)
        init_pair(5, COLOR_RED, COLOR_BLACK);
        // preto no ciano (selecao)
        init_pair(6, COLOR_BLACK, COLOR_CYAN);
    }

    bkgd(COLOR_PAIR(1));
    erase();
    refresh();

    CatalogManager* catalog_manager = catalog_manager_create();

    start_interactive_ui(catalog_manager);

    endwin();

    catalog_manager_destroy(catalog_manager);
    return 0;
}
