#ifndef QUERY3_H
#define QUERY3_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "utils/date.h"
#include <stdbool.h>

/**
 * @brief Executa a Query 3: Aeroporto com mais partidas num intervalo de datas.
 * @return QueryResult* contendo a linha vencedora (code, name, city, country, count).
 */
QueryResult* execute_query3(AirportCatalog* airport_manager, char* start_date_str, char* end_date_str);

#endif