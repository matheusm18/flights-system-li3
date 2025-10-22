#ifndef AIRPORT_H
#define AIRPORT_H

typedef struct airport Airport;

Airport* create_airport(const char* code, const char* name, const char* city, const char* country, double latitude, double longitude, const char* icao, const char* type);

void destroy_airport(Airport* airport);


// getters
const char* airport_get_code(const Airport* airport);
const char* airport_get_name(const Airport* airport);
const char* airport_get_city(const Airport* airport);
const char* airport_get_country(const Airport* airport);
const char* airport_get_type(const Airport* airport);

#endif