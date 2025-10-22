#ifndef QUERY2_H
#define QUERY2_H

#include "catalog/flight_catalog.h"
#include "catalog/aircraft_catalog.h"

void execute_query2(FlightCatalog* flight_manager, AircraftCatalog* aircraft_manager, int n, const char* manufacturer, const char* output_path);

#endif