#include "entities/flight.h"
#include "utils/date.h"
#include <stdlib.h>
#include <string.h>

struct flight {
    char* flight_id;
    DateTime* departure;
    DateTime* actual_departure;
    DateTime* arrival;
    DateTime* actual_arrival;
    char* gate;
    char* status;
    char* origin;
    char* destination;
    char* aircraft;
    char* airline;
    // char* tracking_url; // será necessário armazenar?
};

Flight* create_flight(const char* flight_id,
                      DateTime* departure,
                      DateTime* actual_departure,
                      DateTime* arrival,
                      DateTime* actual_arrival,
                      const char* gate,
                      const char* status,
                      const char* origin,
                      const char* destination,
                      const char* aircraft,
                      const char* airline,
                      const char* tracking_url) {

    Flight* flight = malloc(sizeof(Flight));
    if (!flight) return NULL;

    flight->flight_id = strdup(flight_id);
    flight->departure = departure;
    flight->actual_departure = actual_departure;
    flight->arrival = arrival;
    flight->actual_arrival = actual_arrival;

    flight->gate = strdup(gate);
    flight->status = strdup(status);
    flight->origin = strdup(origin);
    flight->destination = strdup(destination);
    flight->aircraft = strdup(aircraft);
    flight->airline = strdup(airline);

    return flight;
}

void destroy_flight(Flight* f) {
    if (!f) return;
    free(f->flight_id);
    free(f->gate);
    free(f->status);
    free(f->origin);
    free(f->destination);
    free(f->aircraft);
    free(f->airline);

    free(f->departure);
    free(f->actual_departure);
    free(f->arrival);
    free(f->actual_arrival);

    free(f);
}

DateTime* flight_get_actual_departure(const Flight* f) {
    if (!f) return NULL;
    return f->actual_departure;
}

const char* flight_get_origin(const Flight* f) {
    if (!f) return NULL;
    return f->origin;
}

const char* flight_get_status(const Flight* f) {
    if (!f) return NULL;
    return f->status;
}

const char* flight_get_flight_id(const Flight* flight) {
    if (flight == NULL) return NULL;
    return flight->flight_id;
}

