#include "queries/query1.h"
#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//==== Listar o resumo de um aeroporto, consoante o identificador recebido por argumento.
void execute_query1(AirportCatalog* manager, const char* airport_code, const char* output_path) {
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Erro ao abrir ficheiro de output");
        return;
    }
    
    Airport* airport = get_airport_by_code(manager, airport_code);
    
    if (airport != NULL) {
        const char* code = get_airport_code(airport);
        const char* name = get_airport_name(airport);
        const char* city = get_airport_city(airport);
        const char* country = get_airport_country(airport);
        char type = get_airport_type(airport);
        
        fprintf(output_file, "%s,%s,%s,%s,%s\n", code, name, city, country, airport_type_to_string(type));
    }
    else {
        fprintf(output_file,"\n");
    }
    
    fclose(output_file);
}