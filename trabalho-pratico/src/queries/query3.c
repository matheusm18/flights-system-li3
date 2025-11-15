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

bool write_result(const char* output_path, AirportCatalog* airport_manager, const char* best_code, int best_count) {
    FILE* out = fopen(output_path, "w");
    if (!out) return false;

    if (!best_code || best_count == 0) {
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

//======= Aeroporto com mais partidas num intervalo de datas
void execute_query3(AirportCatalog* airport_manager, const char* start_date_str, const char* end_date_str, const char* output_path) {

    if (!airport_manager || !start_date_str || !end_date_str || !output_path) return;

    // converter strings de data para int (YYYYMMDD)
    int start_date = string_to_date(start_date_str);
    int end_date   = string_to_date(end_date_str);

    const char* best_code = NULL;
    int best_count = 0;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, airport_catalog_get_airports(airport_manager)); // todos os aeroportos

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Airport* airport = (Airport*) value;
        int count = 0;
        GPtrArray* flights = airport_get_departing_flights(airport);

        if (flights != NULL) {
            for (guint i = 0; i < flights->len; i++) {
                Flight* flight = g_ptr_array_index(flights, i);
                long adt = get_flight_actual_departure(flight);

                if (adt <= 0) break; // voo cancelado ou "N/A", está ordenado -> podemos parar

                int flight_date = get_date_part(adt);

                if (flight_date > end_date) break;    // array ordenado -> podemos parar
                if (flight_date >= start_date) count++; // dentro do intervalo
            }

        }

        // atualiza melhor aeroporto
        if (best_code == NULL || count > best_count ||
            (count == best_count && strcmp(get_airport_code(airport), best_code) < 0)) {
            best_code = get_airport_code(airport);
            best_count = count;
        }
    }

    // escreve resultado
    write_result(output_path, airport_manager, best_code, best_count);
}