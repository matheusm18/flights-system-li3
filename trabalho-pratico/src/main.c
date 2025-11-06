#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "catalog/catalog_manager.h"

#include "io/parser.h"
#include "io/command_processor.h"
#include "queries/query1.h"
#include "queries/query2.h"

int main(int argc, char **argv) {
    
    // tres args visto que ele considera o nome do programa como primeiro arg
    if (argc != 3) {
        fprintf(stderr, "Usage format: %s <dataset_path> <command_list_path>\n", argv[0]);
        return 1;
    }

    const char* dataset_path = argv[1];
    const char* commands_path = argv[2];

    CatalogManager* catalog_manager = catalog_manager_create(); // criar o catalog

    // carregar datasets
    printf("\nA carregar datasets de: %s\n", dataset_path);
    load_datasets(dataset_path, catalog_manager);

    // processar comandos do ficheiro
    printf("\nA processar inputs de: %s\n", commands_path);
    process_commands(commands_path, catalog_manager);

    printf("\nProcessamento concluído! Verifique a pasta 'resultados/'\n");

    // liberar memória

    catalog_manager_destroy(catalog_manager);
    
    return 0;
}