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
#include "utils/date.h"

typedef struct {
    Date* start;
    Date* end;
    GHashTable* counts; // key = strdup(origin) -> value = int*
} FlightCounts;

int is_cancelled(const char* status) {
    if (!status) return 0;
    return g_strcmp0(status, "Cancelled") == 0;
}

// callback para cada entry na hashtable de flights
void count_number_flights(gpointer key, gpointer value, gpointer user_data) {
    (void) key;
    FlightCounts* counter = (FlightCounts*) user_data;
    Flight* flight = (Flight*) value;

    if (!flight) return;

    DateTime* actual_dt = get_flight_actual_departure(flight);
    if (!actual_dt) return;

    Date actual_date = actual_dt->date_part; // converte o datetime para date

    const char* origin = get_flight_origin(flight);
    const char* status = get_flight_status(flight);

    if (!origin || is_cancelled(status)) return;
    
    if (date_compare(&actual_date, counter->start) < 0 || date_compare(&actual_date, counter->end) > 0) return;

    gpointer existing_count = g_hash_table_lookup(counter->counts, origin);

    if (!existing_count) { // primeira ocorrencia do aeroporto: criar "tabela"
        char* airport_key = g_strdup(origin); 
        int* count = g_new(int, 1);
        *count = 1;
        g_hash_table_insert(counter->counts, airport_key, count);
    } else {
        int* current_count = (int*) existing_count;
        (*current_count)++;
    }
}

void write_empty_result(const char* output_path) {
    FILE* out = fopen(output_path, "w");
    if (out) {
        fprintf(out, "\n");
        fclose(out);
    }
}

void find_best_airport(GHashTable* counts, const char** best_code, int* best_count) {
    *best_code = NULL;
    *best_count = 0;
    
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, counts);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char* code = (const char*) key;
        int count = *((int*) value);
        
        if (count > *best_count || (count == *best_count && (*best_code == NULL || strcmp(code, *best_code) < 0))) {
            *best_code = code;
            *best_count = count;
        }
    }
}

bool write_result(const char* output_path, AirportCatalog* airport_manager, const char* best_code, int best_count) {
    FILE* out = fopen(output_path, "w");
    if (!out) return false;

    if (!best_code) {
        fprintf(out, "\n");
    } else {
        const char* name;
        const char* city;
        const char* country;

        if (airport_manager) {
            Airport* airport = get_airport_by_code(airport_manager, best_code);
            if (airport) {
                name = get_airport_name(airport);
                city = get_airport_city(airport);
                country = get_airport_country(airport);
            }
        }

        fprintf(out, "%s,%s,%s,%s,%d\n", best_code, name, city, country, best_count);
    }

    fclose(out);
    return true;
}

void execute_query3(FlightCatalog* flight_manager, AirportCatalog* airport_manager, const char* start_date_str, const char* end_date_str, const char* output_path) {

    if (!flight_manager || !start_date_str || !end_date_str || !output_path) return;

    Date* start = date_create_from_string(start_date_str);
    Date* end = date_create_from_string(end_date_str);

    if (!start || !end) {
        write_empty_result(output_path);
        if (start) date_destroy(start);
        if (end) date_destroy(end);
        return;
    }

    GHashTable* counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    FlightCounts flight_counts = {.start = start, .end = end, .counts = counts};

    GHashTable* flights = get_flight_catalog(flight_manager);
    if (!flights) {
        write_empty_result(output_path);
        g_hash_table_destroy(counts);
        date_destroy(start);
        date_destroy(end);
        return;
    }

    // para cada voo na hash table flights executa a funcao count number flights e passa o ponteiro &flight_counts
    g_hash_table_foreach(flights, count_number_flights, &flight_counts);

    const char* best_code = NULL;
    int best_count = 0;

    find_best_airport(counts, &best_code, &best_count);

    if (!write_result(output_path, airport_manager, best_code, best_count)) {
        write_empty_result(output_path);
    }

    g_hash_table_destroy(counts);
    date_destroy(start);
    date_destroy(end);
}
