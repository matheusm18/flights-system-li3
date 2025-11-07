#include "queries/query2.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// heap
// estrutura auxiliar para ordenar
struct aircraft_count{
    char* aircraft_id;
    int flight_count;
};

void count_flights_by_aircraft (FlightCatalog* flight_manager, GHashTable* counts,const char* manufacturer_filter, AircraftCatalog* aircraft_catalog){
    GHashTable* flights_catalog = get_flight_catalog(flight_manager);
    GHashTableIter i; // i itera pela GHashtable
    gpointer key, value;

    g_hash_table_iter_init(&i, flights_catalog); // inicializa i para percorrer a hashtable

    while (g_hash_table_iter_next(&i, &key, &value)) {
        Flight* flight = (Flight*) value; // temos de colocar (Flight *) porque GHashtable "armazena" ponteiro genéricos para void*
        const char* status = get_flight_status(flight);

        if (status && g_strcmp0(status, "Cancelled") == 0) continue; // ignora voos cancelados

        const char* aircraft_id = get_aircraft_id_from_flight(flight);
        if (!aircraft_id) continue;

        // filtro do manufacturer
        if (manufacturer_filter != NULL && strlen(manufacturer_filter) > 0) {
            Aircraft* aircraft = get_aircraft_by_identifier(aircraft_catalog, aircraft_id);
            if (!aircraft) continue;
            
            const char* manufacturer = get_aircraft_manufacturer(aircraft);
            if (!manufacturer || g_strcmp0(manufacturer, manufacturer_filter) != 0) {
                continue; // ignora se não é do fabricante desejado
            }
        }

        gpointer stored_key = NULL;
        gpointer stored_val = NULL;

        /* se a chave ja existe, usamos lookup_extended para obter o ponteiro interno e reaproveitamos
           g_hash_table_steal remove a entrada sem libertar a chave, permitindo reinserir o mesmo ponteiro com o novo contador 
        */

        if (g_hash_table_lookup_extended(counts, aircraft_id, &stored_key, &stored_val)) {
            int new_count = GPOINTER_TO_INT(stored_val) + 1;
            g_hash_table_steal(counts, stored_key);
            g_hash_table_insert(counts, stored_key, GINT_TO_POINTER(new_count));
        } else {
            g_hash_table_insert(counts, g_strdup(aircraft_id), GINT_TO_POINTER(1));
        }
    }
}

int compare_aircraft_counts(const void* a, const void* b) {
    AircraftCount* ac1 = (AircraftCount*)a;
    AircraftCount* ac2 = (AircraftCount*)b;
    
    // compara por número de voos (decrescente)
    if (ac2->flight_count != ac1->flight_count) {
        return ac2->flight_count - ac1->flight_count;
    }
    
    // caso de empate 
    return strcmp(ac1->aircraft_id, ac2->aircraft_id); // ordena por ID (crescente: "XB-NIQ0" antes de "XB-OIQ0")
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
    
    // ordena o array
    qsort(array, *number_of_aircrafts, sizeof(AircraftCount), compare_aircraft_counts);
    
    return array;
}


void write_top_n_to_file(AircraftCount* sorted_array, int number_of_aircrafts, int N, AircraftCatalog* aircraft_catalog, FILE* output_file) {
    int limit;
    if(N > 0  && N < number_of_aircrafts){
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

    GHashTable* flight_counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    count_flights_by_aircraft(flight_manager, flight_counts, manufacturer, aircraft_manager);

    int number_of_aircrafts;
    AircraftCount* sorted_array = convert_and_sort(flight_counts, &number_of_aircrafts);

    write_top_n_to_file(sorted_array, number_of_aircrafts, n, aircraft_manager, output_file);

    if (number_of_aircrafts == 0) fprintf(output_file, "\n");

    free_aircraft_count_array(sorted_array, number_of_aircrafts);
    g_hash_table_destroy(flight_counts);
    fclose(output_file);

}