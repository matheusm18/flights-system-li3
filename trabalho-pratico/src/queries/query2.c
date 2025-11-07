#include "queries/query2.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


struct aircraft_count{
    char* aircraft_id;
    int flight_count;
};

GHashTable* count_flights_by_aircraft (FlightCatalog* flight_manager,const char* manufacturer_filter, AircraftCatalog* aircraft_catalog){
    GHashTable* precalculated_counts = get_aircraft_flights_counter(aircraft_catalog);
    if (!precalculated_counts) return NULL;

    GHashTable* result = g_hash_table_new_full(g_str_hash,g_str_equal, g_free, NULL);

    GHashTableIter i; 
    gpointer key, value;

    g_hash_table_iter_init(&i, precalculated_counts);

    // Filtro, só passa o necessário
    while (g_hash_table_iter_next(&i, &key, &value)) {
        const char* aircraft_id = (const char*)key; 
        Aircraft* aircraft = get_aircraft_by_identifier(aircraft_catalog, aircraft_id);

        if (!aircraft) continue;

        
        if (manufacturer_filter && *manufacturer_filter) {
            const char* manufacturer = get_aircraft_manufacturer(aircraft);
            if(!manufacturer || strcmp(manufacturer, manufacturer_filter) != 0){
                continue;
            }
        }

        g_hash_table_insert(result, g_strdup(aircraft_id), GINT_TO_POINTER(GPOINTER_TO_INT(value)));
    }
    return result;
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

AircraftCount* convert_and_sort(GHashTable* counts, int* number_of_aircrafts) {
    *number_of_aircrafts = g_hash_table_size(counts);
    AircraftCount* array = malloc(sizeof(AircraftCount) * (*number_of_aircrafts));
    
    GHashTableIter i;
    gpointer key, value;
    int index = 0;
    
    g_hash_table_iter_init(&i, counts);
    while (g_hash_table_iter_next(&i, &key, &value)) {
        array[index].aircraft_id = g_strdup((const char*)key);
        array[index].flight_count = GPOINTER_TO_INT(value);
        index++;
    }
    
    qsort(array, *number_of_aircrafts, sizeof(AircraftCount), compare_aircraft_counts);
    
    return array;
}


void write_top_n_to_file(AircraftCount* sorted_array, int number_of_aircrafts, int N, AircraftCatalog* aircraft_catalog, FILE* output_file) {
    int limit;
    if (N > 0 && N < number_of_aircrafts) {
        limit = N;
    } else {
        limit = number_of_aircrafts;
    }

    for (int i = 0; i < limit; i++) {
        const char* aircraft_id = sorted_array[i].aircraft_id;
        int flight_count = sorted_array[i].flight_count;

        Aircraft* aircraft = get_aircraft_by_identifier(aircraft_catalog, aircraft_id);
        
        if (aircraft) {            
            fprintf(output_file, "%s,%s,%s,%d\n", aircraft_id, get_aircraft_manufacturer(aircraft), get_aircraft_model(aircraft), flight_count);
        }
    }
}

void free_aircraft_count_array(AircraftCount* array, int count) {
    for (int i = 0; i < count; i++) {
        free(array[i].aircraft_id);
    }
    free(array);
}

//======= Top N aeronaves com mais voos
void execute_query2(FlightCatalog* flight_manager, AircraftCatalog* aircraft_manager, int n, const char* manufacturer, const char* output_path) {
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Erro ao abrir ficheiro de output");
        return;
    }

    GHashTable* precalculated_counts = get_aircraft_flights_counter(aircraft_manager);

    if (!precalculated_counts) {
        fprintf(output_file, "\n");
        fclose(output_file);
        return;
    }

    GHashTable* flight_counts = count_flights_by_aircraft(flight_manager, manufacturer, aircraft_manager);

    if (!flight_counts || g_hash_table_size(flight_counts) == 0) {
        fprintf(output_file, "\n");
        if (flight_counts) g_hash_table_destroy(flight_counts);
        fclose(output_file);
        return;
    }

    int number_of_aircrafts;
    AircraftCount* sorted_array = convert_and_sort(flight_counts, &number_of_aircrafts);

    if (!sorted_array || number_of_aircrafts == 0) {
        fprintf(output_file, "\n");
    } else {
        write_top_n_to_file(sorted_array, number_of_aircrafts, n, aircraft_manager, output_file);
    }

    if (sorted_array) free_aircraft_count_array(sorted_array, number_of_aircrafts);
    g_hash_table_destroy(flight_counts); 
    fclose(output_file);

}