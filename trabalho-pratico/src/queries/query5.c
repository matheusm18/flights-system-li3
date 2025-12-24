#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "queries/query5.h"
#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "entities/flight.h"
#include "entities/airport.h"
#include "utils/date.h"
#include "utils/utils.h"

QueryResult* execute_query5(FlightCatalog* flight_manager, int N) {
    
    airline_stats_sort_array(flight_manager);
    QueryResult *res = create_query_result();
    int len = get_airline_stats_array_length(flight_manager);

    for (int i = 0; i < N && i < len; i++) {
        AirlineStats* s = get_airline_stats_from_array(flight_manager, i);

        if (s != NULL) {
            int delayed_flights = get_airline_stats_flights_count(s);
            double media = (double)get_airline_stats_total_delay(s) / delayed_flights;
            char** tokens = malloc(3 * sizeof(char *));
            tokens[0] = get_airline_stats_airline(s);
            tokens[1] = int_to_string(delayed_flights);
            tokens[2] = double_to_string(media);

            add_line_to_result(res, tokens, 3);
        }
    }

    return res;
}