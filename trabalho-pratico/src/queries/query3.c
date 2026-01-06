#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "queries/query3.h"
#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "entities/flight.h"
#include "entities/airport.h"
#include "utils/validation_utils.h"

//======= Aeroporto com mais partidas num intervalo de datas
QueryResult* execute_query3(AirportCatalog* airport_manager, char* start_date_str, char* end_date_str) {

    if (!airport_manager || !start_date_str || !end_date_str) return NULL;

    // converter strings de data para int (YYYYMMDD)
    int start_date = string_to_date(start_date_str);
    int end_date   = string_to_date(end_date_str);

    char* best_code = NULL;
    int best_count = 0;

    AirportIter* air_it = airport_catalog_iter_create(airport_manager);

    const AirportData* data;

    while ((data = airport_catalog_iter_next(air_it)) != NULL) {
        const Airport* airport = get_airport_from_data(data);
        char* current_code = get_airport_code(airport);


        int count = airport_catalog_count_flights_in_range(data, start_date, end_date);

        // atualiza melhor aeroporto
        if (count > best_count || 
            (count == best_count && best_code && strcmp(current_code, best_code) < 0) ||
            (count > 0 && best_code == NULL)) {
            
            free(best_code);

            best_code = current_code;
            best_count = count;
        }
        else free(current_code);
    }

    airport_catalog_iter_free(air_it); // libertar o iterador de aeroportos

    QueryResult* res = create_query_result();

    if (best_code != NULL && best_count > 0) {
        const Airport* best_airport = get_airport_by_code(airport_manager, best_code);
        
        if (best_airport) {
            char** tokens = malloc(5 * sizeof(char*));
            
            tokens[0] = best_code;
            tokens[1] = get_airport_name(best_airport);
            tokens[2] = get_airport_city(best_airport);
            tokens[3] = get_airport_country(best_airport);
            tokens[4] = int_to_string(best_count); // função auxiliar da "utils.c" que já aloca memória para a string

            add_line_to_result(res, tokens, 5);

            best_code = NULL;
        }
    }

    if (best_code != NULL) free(best_code);

    return res;
}