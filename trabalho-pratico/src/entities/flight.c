#include "entities/flight.h"
#include "utils/utils_validation.h"
#include <stdlib.h>
#include <string.h>

struct flight {
    char flight_id[9];
    long departure;
    long actual_departure;
    //long arrival;
    //long actual_arrival;
    //char* gate;
    char* status;
    char origin[4];
    char destination[4];
    char* aircraft;
    //char* airline;
    //char* tracking_url;
};

Flight* create_flight(char* flight_id, long departure, long actual_departure, long arrival, long actual_arrival, const char* gate, const char* status, 
                      const char* origin, const char* destination, const char* aircraft, const char* airline) {

    (void) arrival;
    (void) actual_arrival;
    (void) gate;
    (void) airline;
    
    Flight* flight = malloc(sizeof(Flight));
    if (!flight) return NULL;
    
    memset(flight, 0, sizeof(Flight));
    strncpy(flight->flight_id, flight_id, 8);
    strncpy(flight->origin, origin, 3);
    strncpy(flight->destination, destination, 3);

    flight->departure = departure;
    flight->actual_departure = actual_departure;
    flight->status = (char*) status;
    flight->aircraft = (char*) aircraft;

    return flight;
}

void destroy_flight(Flight* f) {
    if (!f) return;

    free(f);
}

long get_flight_actual_departure(const Flight* f) {
    return f->actual_departure;
}

long get_flight_departure(const Flight* f) {
    return f->departure;
}

char* get_flight_origin(const Flight* f) {
    if (!f) return NULL;
    return strdup(f->origin);
}

char* get_flight_destination(const Flight* f) {
    if (!f) return NULL;
    return strdup(f->destination);
}

char* get_flight_status(const Flight* f) {
    if (!f) return NULL;
    return strdup(f->status);
}

char* get_flight_id(const Flight* flight) {
    if (flight == NULL) return NULL;
    return strdup(flight->flight_id);
}

char* get_aircraft_id_from_flight(const Flight* flight) {
    if (flight == NULL || flight->aircraft == NULL) {
        return NULL;
    }
    
    return strdup(flight->aircraft);
}
