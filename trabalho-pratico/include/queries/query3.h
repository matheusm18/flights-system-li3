#ifndef QUERY3_H
#define QUERY3_H

#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"

void execute_query3(FlightCatalog* flight_manager, AirportCatalog* airport_manager, const char* start_date_str, const char* end_date_str, const char* output_path);

#endif