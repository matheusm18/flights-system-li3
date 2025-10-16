#include "entities/flight.h"
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
    char* tracking_url; // será necessário armazenar?
};

Flight* create_flight(const char* flight_id, Date* departure, Date* actual_departure, Date* arrival, Date* actual_arrival, const char* gate, const char* status, const char* origin, 
                      const char* destination, const char* aircraft, const char* airline, const char* tracking_url) {

    Flight* flight = malloc(sizeof(Flight));
    if (flight == NULL) return NULL;

    flight->flight_id = strdup(flight_id);
    flight->departure = date_create(date_get_year(departure),date_get_month(departure), date_get_day(departure));
    flight->actual_departure = date_create(date_get_year(actual_departure),date_get_month(actual_departure), date_get_day(actual_departure));
    flight->arrival = date_create(date_get_year(arrival),date_get_month(arrival), date_get_day(arrival));
    flight->actual_arrival = date_create(date_get_year(actual_arrival),date_get_month(actual_arrival), date_get_day(actual_arrival));
    flight->gate = strdup(gate);
    flight->status = strdup(status);
    flight->origin = strdup(origin);
    flight->destination = strdup(destination);
    flight->aircraft = strdup(aircraft);
    flight->airline = strdup(airline);
    flight->tracking_url = strdup(tracking_url);

    return flight;
}

void destroy_flight(Flight* f) {
    if (f != NULL) {

        free(f->flight_id);

        date_destroy(f->departure);
        date_destroy(f->actual_departure);
        date_destroy(f->arrival);
        date_destroy(f->actual_arrival);

        free(f->gate);
        free(f->status);
        free(f->origin);
        free(f->destination);
        free(f->aircraft);
        free(f->airline);
        free(f->tracking_url);

        free(f);
    }
}