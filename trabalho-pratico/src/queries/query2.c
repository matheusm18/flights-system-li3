#include "queries/query2.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>



void count_flights_by_aircraft (FlightCatalog* flight_manager, GHashTable* counts){
    GHashTable* flights_catalog = get_flight_catalog(flight_manager);
    GHashTableIter i; // i itera pela GHashtable
    gpointer key, value;

    g_hash_table_iter_init(&i, flights_catalog); // inicializa i para percorrer a hashtable

    while (g_hash_table_iter_next(&i, &key, &value)) {
        Flight* flight = (Flight*) value; // temos de colocar (Flight *) porque GHashtable "armazena" ponteiro genéricos para void*
        const char* status = get_flight_status(flight);


        if (status && g_strcmp0(status, "cancelled") == 0) continue; // ignora voos cancelados

        const char* aircraft_id = get_aircraft_id_from_flight(flight);
        if (!aircraft_id) continue;

        gpointer current_flight_count = g_hash_table_lookup(counts, aircraft_id);
        int new_count;

        if (current_flight_count != NULL) {
            int old_count = GPOINTER_TO_INT(current_flight_count);
            new_count = old_count + 1;
        } else {
            new_count = 1;
        }

        g_hash_table_replace(counts, g_strdup(aircraft_id), GINT_TO_POINTER(new_count)); // replace ou insert?
        
    }
}


void write_to_file(GHashTable* counts, FILE* output_file) {
    GHashTableIter i;
    gpointer key, value;

    g_hash_table_iter_init(&i, counts); 

    // Itera sobre a GHashTable de resultados
    while (g_hash_table_iter_next(&i, &key, &value)) {
        const char* aircraft_id = (const char*) key; 
        int flight_count = GPOINTER_TO_INT(value);

        fprintf(output_file, "%s,%d\n", aircraft_id, flight_count);
    }
}


//======= Top N aeronaves com mais voos
void execute_query2(FlightCatalog* flight_manager, AircraftCatalog* aircraft_manager, int n, const char* manufacturer, const char* output_path) {
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Erro ao abrir ficheiro de output");
        return;
    }

    GHashTable* flight_counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    count_flights_by_aircraft(flight_manager, flight_counts);

    write_flight_counts_to_file(flight_counts, output_file);

    g_hash_table_destroy(flight_counts);
    fclose(output_file);

}