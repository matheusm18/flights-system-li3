#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/parser.h"
#include "io/command_processor.h"
#include "interactive/interactive_mode.h"

int main(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s", argv[0]);
        return 1;
    }

    CatalogManager* catalog_manager = catalog_manager_create(); // criar o catalog

    start_interactive_ui(catalog_manager);
    printf("\nPrograma encerrado!\n");

    catalog_manager_destroy(catalog_manager);

    return 0;
}