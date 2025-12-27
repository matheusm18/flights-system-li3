#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "queries/query6.h"
#include "catalog/reservation_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "entities/reservation.h"
#include "entities/passenger.h"
#include "entities/flight.h"
#include "entities/airport.h"
#include "utils/utils_validation.h"

QueryResult* execute_query6(ReservationCatalog* manager, char* nationality) {
    QueryResult* res = create_query_result();
    if (!manager || !nationality) return res;

    GHashTable* airport_map = reservation_catalog_get_nationality_map(manager, nationality);

    if (!airport_map) return res;

    GHashTableIter iter;
    gpointer key, value;

    char* best_airport = NULL;
    int best_count = -1;

    g_hash_table_iter_init(&iter, airport_map);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* airport_id = key;
        int count = *(int*) value;

        if (count > best_count ||
           (count == best_count &&
            strcmp(airport_id, best_airport) < 0)) {

            best_airport = airport_id;
            best_count = count;
        }
    }

    if (best_airport) {
        char** tokens = malloc(2 * sizeof(char*));
        tokens[0] = strdup(best_airport);
        tokens[1] = int_to_string(best_count);

        add_line_to_result(res, tokens, 2);
    }

    return res;
}


