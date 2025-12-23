#include "queries/query1.h"
#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//==== Listar o resumo de um aeroporto, consoante o identificador recebido por argumento.
QueryResult* execute_query1(AirportCatalog* manager, char* airport_code) {
    
    const Airport* airport = get_airport_by_code(manager, airport_code);
    QueryResult *res = create_query_result();
    
    if (airport != NULL) {
        char** tokens = malloc(5 * sizeof(char*));
        tokens[0] = get_airport_code(airport);
        tokens[1] = get_airport_name(airport);
        tokens[2] = get_airport_city(airport);
        tokens[3] = get_airport_country(airport);
        tokens[4] = get_airport_type(airport);
        
        add_line_to_result(res, tokens, 5);
    }
    return res;
}