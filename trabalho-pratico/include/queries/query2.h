#ifndef QUERY2_H
#define QUERY2_H

#include "catalog/flight_catalog.h"
#include "catalog/aircraft_catalog.h"
#include <stdio.h>

typedef struct aircraft_flight_count AircraftFlightCount;

void count_flights_by_aircraft (FlightCatalog* flight_manager, GHashTable* counts);
void write_to_file(GHashTable* counts, FILE* output_file);
void execute_query2(FlightCatalog* flight_manager, AircraftCatalog* aircraft_manager, int n, const char* manufacturer, const char* output_path);

#endif