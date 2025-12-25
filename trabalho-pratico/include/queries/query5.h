#ifndef QUERY5_H
#define QUERY5_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "utils/utils_validation.h"
#include <stdbool.h>

void prepare_airline_stats_for_sorting(FlightCatalog* flight_manager);
QueryResult* execute_query5(FlightCatalog* flight_manager, int N);

#endif