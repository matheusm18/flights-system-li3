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
#include "utils/validation_utils.h"

QueryResult* execute_query6(ReservationCatalog* manager, char* nationality) {
    QueryResult* res = create_query_result();
    if (!manager || !nationality) return res;

    ReservationStatsIter* it = reservation_catalog_create_stats_iter(manager, nationality);
    if (!it) return res;

    const char* best_airport = NULL; 
    int best_count = -1;

    const char* current_airport;
    int current_count;
    while (reservation_catalog_stats_iter_next(it, &current_airport, &current_count)) {
        
        if (current_count > best_count) {
            best_count = current_count;
            best_airport = current_airport;
        } 
        else if (current_count == best_count) {
            if (strcmp(current_airport, best_airport) < 0) {
                best_airport = current_airport;
            }
        }
    }

    reservation_catalog_stats_iter_destroy(it);

    if (best_airport) {
        char** tokens = malloc(2 * sizeof(char*));
        
        tokens[0] = strdup(best_airport); 
        tokens[1] = int_to_string(best_count);

        add_line_to_result(res, tokens, 2);
    }

    return res;
}