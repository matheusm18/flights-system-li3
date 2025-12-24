#ifndef QUERY5_H
#define QUERY5_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "utils/date.h"
#include <stdbool.h>

QueryResult* execute_query5(FlightCatalog* flight_manager, int N);

#endif