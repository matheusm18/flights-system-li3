#include "entities/aircraft.h"
#include <stdlib.h>
#include <string.h>

struct aircraft {
    char identifier[9];
    char* manufacturer;
    char* model;
    unsigned short year;
    //unsigned short capacity;
    //unsigned short range;
    int flight_count;
};

Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, unsigned short year, unsigned short capacity, unsigned short range) {
    (void) capacity;
    (void) range;

    Aircraft* aircraft = malloc(sizeof(Aircraft));
    if (aircraft == NULL) return NULL;

    strncpy(aircraft->identifier, identifier, 9);
    aircraft->identifier[8] = '\0';
    aircraft->manufacturer = strdup(manufacturer);
    aircraft->model = strdup(model);
    aircraft->year = year;
    //aircraft->capacity = capacity;
    //aircraft->range = range;
    aircraft->flight_count = 0;

    return aircraft;
}

void destroy_aircraft(Aircraft* a) {
    if (a != NULL) {

        free(a->manufacturer);
        free(a->model);

        free(a);
    }
}

const char* get_aircraft_identifier(const Aircraft* a) { 
    if (a == NULL) return NULL;
    return a->identifier; 
}

const char* get_aircraft_manufacturer(const Aircraft* a) {
    if (a == NULL) return NULL;
    return a->manufacturer;
}

const char* get_aircraft_model(const Aircraft* a) {
    if (a == NULL) return NULL;
    return a->model;
}

int get_aircraft_flight_count(const Aircraft* a) {
    if (a == NULL) return 0;
    return a->flight_count;
}

void aircraft_increment_flight_count(Aircraft* aircraft) {
    if (aircraft != NULL) {
        aircraft->flight_count++;
    }
}
