#include "io/parser.h"
#include "io/airport_parser.h"
#include "utils/validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void load_datasets(const char* dataset_path) {

    char airports_file[256];
    snprintf(airports_file, sizeof(airports_file), "%s/airports.csv", dataset_path);
    
    printf("A carregar os aeroportos de: %s\n", airports_file);
    ler_csv(airports_file, processar_linha_valida);
    printf("\nTodos os aeroportos válidos foram carregados!\n");
}