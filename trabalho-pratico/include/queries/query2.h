#ifndef QUERY2_H
#define QUERY2_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include <stdio.h>

typedef struct aircraft_count AircraftCount;

/**
 * @brief Executa a Query 2: Top N aeronaves com mais voos
 * @return QueryResult* contendo os tokens para escrita posterior
 */
QueryResult* execute_query2(AircraftCatalog* aircraft_manager, int n, char* manufacturer);

int filter_build_aircraft_count_array(AircraftCatalog* aircraft_catalog, char* manufacturer, AircraftCount** out_array);
int compare_aircraft_counts(const void* a, const void* b);
void free_aircraft_count_array(AircraftCount* array, int count);


#endif