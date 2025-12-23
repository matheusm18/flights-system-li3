#ifndef QUERY1_H
#define QUERY1_H

#include "queries/query_result.h"
#include "catalog/airport_catalog.h"

QueryResult* execute_query1(AirportCatalog* manager, char* airport_code);

#endif