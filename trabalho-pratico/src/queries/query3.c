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

GHashTable* filter_by_date_range(GHashTable* precalculated_data, int start_date, int end_date) {
    if (!precalculated_data) return NULL;

    GHashTable* result = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    
    GHashTableIter airport_iter;
    gpointer airport_key, dates_hash;
    
    g_hash_table_iter_init(&airport_iter, precalculated_data);

    while (g_hash_table_iter_next(&airport_iter, &airport_key, &dates_hash)) {
        const char* airport_code = (const char*)airport_key;
        GHashTable* dates = (GHashTable*)dates_hash;
        
        int total_count = 0;
        
        GHashTableIter date_iter;
        gpointer date_key, count_value;
        g_hash_table_iter_init(&date_iter, dates);
        
        // Soma contadores 
        while (g_hash_table_iter_next(&date_iter, &date_key, &count_value)) {
            int date = GPOINTER_TO_INT(date_key);
            
            if (date >= start_date && date <= end_date) {
                total_count += GPOINTER_TO_INT(count_value);
            }
        }
        
        if (total_count > 0) {
            g_hash_table_insert(result, g_strdup(airport_code), GINT_TO_POINTER(total_count));
        }
    }
    
    return result;
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
        int count = GPOINTER_TO_INT(value);
        
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
        const char* name = "";
        const char* city = "";
        const char* country = "";

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

    // converter strings de data para int (YYYYMMDD)
    int start_date = string_to_date(start_date_str);
    int end_date = string_to_date(end_date_str);

    GHashTable* precalculated_data = get_flights_by_origin(airport_manager);
    
    if (!precalculated_data || g_hash_table_size(precalculated_data) == 0) {
        write_empty_result(output_path);
        return;
    }

    GHashTable* filtered_counts = filter_by_date_range(precalculated_data, start_date, end_date);

    if (!filtered_counts || g_hash_table_size(filtered_counts) == 0) {
        write_empty_result(output_path);
        if (filtered_counts) g_hash_table_destroy(filtered_counts);
    
        return;
    }

    // Encontrar melhor aeroporto
    const char* best_code = NULL;
    int best_count = 0;
    find_best_airport(filtered_counts, &best_code, &best_count);

    // Escrever resultado
    if (!write_result(output_path, airport_manager, best_code, best_count)) {
        write_empty_result(output_path);
    }

    g_hash_table_destroy(filtered_counts);
}

