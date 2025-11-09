#ifndef FLIGHT_H
#define FLIGHT_H

#include "utils/date.h"

typedef struct flight Flight;

Flight* create_flight(const char* flight_id, long departure, long actual_departure, long arrival, long actual_arrival,
                      const char* gate, const char* status, const char* origin, const char* destination, const char* aircraft, const char* airline);

void destroy_flight(Flight* f);

long get_flight_actual_departure(const Flight* f);
const char* get_flight_id(const Flight* flight);
const char* get_flight_origin(const Flight* f);
const char* get_flight_status(const Flight* f);
const char* get_aircraft_id_from_flight(const Flight* f);

#endif
