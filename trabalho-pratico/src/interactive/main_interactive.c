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
    keypad(stdscr, TRUE);

    start_color();
    use_default_colors();

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);

    attrset(COLOR_PAIR(1)); 

    CatalogManager* catalog_manager = catalog_manager_create();

    start_interactive_ui(catalog_manager);

    endwin();

    catalog_manager_destroy(catalog_manager);
    return 0;
}
