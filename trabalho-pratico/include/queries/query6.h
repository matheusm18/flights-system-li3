#ifndef QUERY6_H
#define QUERY6_H

#include "queries/query_result.h"
#include "catalog/reservation_catalog.h"
#include "utils/utils_validation.h"
#include <stdbool.h>

QueryResult* execute_query6(ReservationCatalog* manager, char* nationality);

#endif