#include "queries/query2.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include "utils/validation_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils/query2_utils.h"



//======= Top N aeronaves com mais voos
QueryResult* execute_query2(AircraftCatalog* aircraft_manager, int n, char* manufacturer) {
    QueryResult* res = create_query_result();

    if (n <= 0) return res;
    
    MinHeap* heap = create_min_heap(n);
    if(!heap) return res;

    AircraftIter* iter = aircraft_catalog_iter_create(aircraft_manager);

    const AircraftData* data;

    while((data = aircraft_catalog_iter_next(iter)) != NULL){
        const Aircraft* aircraft = get_aircraft_from_data(data);

        if(manufacturer && *manufacturer){
            char* manuf = get_aircraft_manufacturer(aircraft);
            if(!manuf || g_strcmp0(manuf, manufacturer) != 0){
                free(manuf);
                continue;
            }
            free(manuf);
        }
        
        char* aircraft_id = get_aircraft_identifier(aircraft);

        int flight_count = get_aircraft_flight_count(data);

        insert_min_heap(heap, aircraft_id, flight_count);

        free(aircraft_id);
    }

    aircraft_catalog_iter_free(iter);

    sort_desc_min_heap(heap); // só vai ordenar n elementos

    int limit = min_heap_size(heap);

    for (int i = 0; i < limit; i++) {
        const char* aircraft_id = get_aicraft_id_min_heap(heap, i);

        int flight_count = get_flight_count_min_heap(heap, i);

        const Aircraft* a = get_aircraft_by_identifier(aircraft_manager,aircraft_id);

        if (!a) continue;

        char** tokens = malloc(4 * sizeof(char*));
        tokens[0] = strdup(aircraft_id);
        tokens[1] = get_aircraft_manufacturer(a);
        tokens[2] = get_aircraft_model(a);
        tokens[3] = int_to_string(flight_count);

        add_line_to_result(res, tokens, 4);
    }

    destroy_min_heap(heap);


    return res;
}