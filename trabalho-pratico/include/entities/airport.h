#ifndef AIRPORT_H
#define AIRPORT_H

#include "entities/flight.h"
#include <glib.h>

typedef struct airport Airport;

Airport* create_airport(const char *code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type);

void destroy_airport(Airport* airport);
const char* airport_type_to_string(char type);
void airport_add_departing_flight(Airport* airport, Flight* flight);
int compare_flight_actual_departure(const void* a, const void* b);

// getters
const char* get_airport_code(const Airport* airport);
const char* get_airport_name(const Airport* airport);
const char* get_airport_city(const Airport* airport);
const char* get_airport_country(const Airport* airport);
char get_airport_type(const Airport* airport);
char get_airport_type_char(const char* type_str);
GPtrArray* airport_get_departing_flights(Airport* airport);

#endif
