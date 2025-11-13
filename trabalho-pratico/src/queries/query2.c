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

// função que irá filtrar e construir o array de AircraftCount
int filter_build_aircraft_count_array(AircraftCatalog* aircraft_catalog, const char* manufacturer_filter, AircraftCount** out_array) {
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

    GHashTableIter iter;
    gpointer key, value;
    int index = 0;

    g_hash_table_iter_init(&iter, aircraft_catalog_get_aircrafts(aircraft_catalog));
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Aircraft* aircraft = (Aircraft*)value;
        if (!aircraft) continue;

        if (manufacturer_filter && *manufacturer_filter) {
            const char* manufacturer = get_aircraft_manufacturer(aircraft);
            if (!manufacturer || g_strcmp0(manufacturer, manufacturer_filter) != 0) continue;
        }

        array[index].aircraft_id = g_strdup(get_aircraft_identifier(aircraft));
        array[index].flight_count = get_aircraft_flight_count(aircraft);
        index++;
    }

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

void write_top_n_aircraft(FILE* output_file, AircraftCatalog* aircraft_catalog, AircraftCount* array, int number_of_aircrafts, int N) {
    if (!array || number_of_aircrafts == 0) {
        fprintf(output_file, "\n");
        return;
    }

    int limit;
    if (N > 0 && N < number_of_aircrafts) {
        limit = N;
    } else {
        limit = number_of_aircrafts;
    }

     if (number_of_aircrafts > 0) qsort(array, number_of_aircrafts, sizeof(AircraftCount), compare_aircraft_counts);

    for (int i = 0; i < limit; i++) {
        Aircraft* a = get_aircraft_by_identifier(aircraft_catalog, array[i].aircraft_id);
        if (a) {
            fprintf(output_file, "%s,%s,%s,%d\n", array[i].aircraft_id, get_aircraft_manufacturer(a), get_aircraft_model(a), array[i].flight_count);
        }
    }
}

void free_aircraft_count_array(AircraftCount* array, int count) {
    if (!array) return;

    for (int i = 0; i < count; i++) {
        free(array[i].aircraft_id);
    }
    free(array);
}

//======= Top N aeronaves com mais voos
void execute_query2(AircraftCatalog* aircraft_manager, int n, const char* manufacturer, const char* output_path) {
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Erro ao abrir ficheiro de output");
        return;
    }

    AircraftCount* array = NULL;
    int count = filter_build_aircraft_count_array(aircraft_manager, manufacturer, &array);

    write_top_n_aircraft(output_file, aircraft_manager, array, count, n);

    free_aircraft_count_array(array, count);

    fclose(output_file);
}