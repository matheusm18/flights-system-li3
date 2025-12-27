#ifndef QUERY4_H
#define QUERY4_H

#include "queries/query_result.h"
#include "catalog/reservation_catalog.h"
#include "catalog/passenger_catalog.h"

QueryResult* execute_query4(ReservationCatalog* reservation_catalog, PassengerCatalog* passenger_catalog, char* begin_date, char* end_date);

#endif