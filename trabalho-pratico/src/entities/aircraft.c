#include "entities/aircraft.h"
#include <stdlib.h>
#include <string.h>

struct aircraft {
    char* identifier;
    char* manufacturer;
    char* model;
    int year;
    int capacity;
    int range;
};

Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, int year, int capacity, int range) {

    Aircraft* aircraft = malloc(sizeof(Aircraft));
    if (aircraft == NULL) return NULL;

    aircraft->identifier = strdup(identifier);
    aircraft->manufacturer = strdup(manufacturer);
    aircraft->model = strdup(model);
    aircraft->year = year;
    aircraft->capacity = capacity;
    aircraft->range = range;

    return aircraft;
}

void destroy_aircraft(Aircraft* a) {
    if (a != NULL) {

        free(a->identifier);
        free(a->manufacturer);
        free(a->model);

        free(a);
    }
}

const char aircraft_get_identifier(Aircraft* a){
    if (a == NULL) return NULL;
    return a->identifier;
}
