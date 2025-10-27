#include "queries/query1.h"
#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void execute_query1(AirportCatalog* manager, const char* airport_code, const char* output_path) {
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Erro ao abrir ficheiro de output");
        return;
    }
    
    Airport* airport = airport_catalog_get_by_code(manager, airport_code);
    
    if (airport != NULL) {
        const char* code = airport_get_code(airport);
        const char* name = airport_get_name(airport);
        const char* city = airport_get_city(airport);
        const char* country = airport_get_country(airport);
        const char* type = airport_get_type(airport);
        
        fprintf(output_file, "%s,%s,%s,%s,%s\n", code, name, city, country, type);
    }
    
    fclose(output_file);
}