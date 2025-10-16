#ifndef AIRPORT_H
#define AIRPORT_H

typedef struct airport Airport;

Airport* create_airport(const char* code, const char* name, const char* city, const char* country, double latitude, double longitude, const char* icao, const char* type);

void destroy_airport(Airport* airport);

#endif