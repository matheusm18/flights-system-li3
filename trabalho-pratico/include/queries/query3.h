#ifndef QUERY3_H
#define QUERY3_H

#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "utils/date.h"
#include <stdbool.h>


GHashTable* filter_by_date_range(GHashTable* precalculated_data, int start_date, int end_date);
void write_empty_result(const char* output_path) ;
void find_best_airport(GHashTable* counts, const char** best_code, int* best_count);
bool write_result(const char* output_path, AirportCatalog* airport_manager, const char* best_code, int best_count);
void execute_query3(FlightCatalog* flight_manager, AirportCatalog* airport_manager, const char* start_date_str, const char* end_date_str, const char* output_path);

#endif