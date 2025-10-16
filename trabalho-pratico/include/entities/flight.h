#ifndef FLIGHT_H
#define FLIGHT_H

#include "utils/date.h"

typedef struct flight Flight;

Flight* create_flight(const char* flight_id, Date* departure, Date* actual_departure, Date* arrival, Date* actual_arrival, const char* gate, const char* status, const char* origin, 
                      const char* destination,const char* aircraft, const char* airline, const char* tracking_url);

void destroy_flight(Flight* f);

#endif