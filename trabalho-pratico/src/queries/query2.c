#include "queries/query2.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include "utils/utils_validation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct aircraft_count{
    char* aircraft_id;
    int flight_count;
};

// função que irá filtrar e construir o array de AircraftCount
int filter_build_aircraft_count_array(AircraftCatalog* aircraft_catalog, char* manufacturer_filter, AircraftCount** out_array) {
    int total_aircrafts = get_total_aircrafts_in_catalog(aircraft_catalog);
    if (total_aircrafts == 0) {
        *out_array = NULL;
        return 0;
    }

    AircraftCount* array = malloc(sizeof(AircraftCount) * total_aircrafts);
    if (!array) {
        *out_array = NULL;
        return 0;
    }

    AircraftIter* iter = aircraft_catalog_iter_create(aircraft_catalog);
    const AircraftData* data;

    int index = 0;
    while((data = aircraft_catalog_iter_next(iter)) != NULL) {
        const Aircraft* aircraft = get_aircraft_from_data(data);

        if (manufacturer_filter && *manufacturer_filter) {
            char* manufacturer = get_aircraft_manufacturer(aircraft);
            if (!manufacturer || g_strcmp0(manufacturer, manufacturer_filter) != 0) {
                free(manufacturer);
                continue;
            }
            free(manufacturer);
        }

        array[index].aircraft_id = get_aircraft_identifier(aircraft);
        array[index].flight_count = get_aircraft_flight_count(data);
        index++;
    }

    aircraft_catalog_iter_free(iter); // libertar memoria do iterador
    *out_array = array;
    return index; // número de aeronaves no array
}

int compare_aircraft_counts(const void* a, const void* b) {
    AircraftCount* ac1 = (AircraftCount*)a;
    AircraftCount* ac2 = (AircraftCount*)b;
    
    if (ac2->flight_count != ac1->flight_count) {
        return ac2->flight_count - ac1->flight_count;
    }
    
    // caso de empate 
    return strcmp(ac1->aircraft_id, ac2->aircraft_id);
}

//======= Top N aeronaves com mais voos
QueryResult* execute_query2(AircraftCatalog* aircraft_manager, int n, char* manufacturer) {

    AircraftCount* array = NULL;
    int number_of_aircrafts = filter_build_aircraft_count_array(aircraft_manager, manufacturer, &array);

    // determinar o limite real (N ou o total disponível)
    int limit = (n < number_of_aircrafts) ? n : number_of_aircrafts;

    QueryResult* res = create_query_result();

    if (number_of_aircrafts > 0) qsort(array, number_of_aircrafts, sizeof(AircraftCount), compare_aircraft_counts);

    // construir as linhas de resultado (tokens)
    for (int i = 0; i < limit; i++) {

        const Aircraft* a = get_aircraft_by_identifier(aircraft_manager, array[i].aircraft_id);
        
        if (a) {
            // 4 tokens: id, fabricante, modelo, contagem
            char** tokens = malloc(4 * sizeof(char*));
            
            tokens[0] = strdup(array[i].aircraft_id);
            tokens[1] = get_aircraft_manufacturer(a);
            tokens[2] = get_aircraft_model(a);
            tokens[3] = int_to_string(array[i].flight_count);

            add_line_to_result(res, tokens, 4);
        }
    }

    // libertar memoria
    if (array) {
        for (int i = 0; i < number_of_aircrafts; i++) free(array[i].aircraft_id); 
        free(array);
    }

    return res;
}