#ifndef QUERY2_H
#define QUERY2_H

#include "catalog/flight_catalog.h"
#include "catalog/aircraft_catalog.h"
#include <stdio.h>

typedef struct aircraft_count AircraftCount;

void count_flights_by_aircraft (FlightCatalog* flight_manager, GHashTable* counts, const char* manufacturer_filter, AircraftCatalog* aircraft_catalog);
int compare_aircraft_counts(const void* a, const void* b);
AircraftCount* convert_and_sort(GHashTable* counts, int* number_of_aircrafts);
void write_top_n_to_file(AircraftCount* sorted_array, int number_of_aircrafts, int N, AircraftCatalog* aircraft_catalog, FILE* output_file);
void free_aircraft_count_array(AircraftCount* array, int count);
void execute_query2(FlightCatalog* flight_manager, AircraftCatalog* aircraft_manager, int n, const char* manufacturer, const char* output_path);


#endif