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
};

Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, unsigned short year, unsigned short capacity, unsigned short range) {
    (void) capacity;
    (void) range;

    Aircraft* aircraft = malloc(sizeof(Aircraft));
    if (aircraft == NULL) return NULL;

    strncpy(aircraft->identifier, identifier, 8);

    aircraft->identifier[8] = '\0';
    aircraft->manufacturer = (char*) manufacturer;
    aircraft->model = (char*) model;
    aircraft->year = year;
    //aircraft->capacity = capacity;
    //aircraft->range = range;

    return aircraft;
}

void destroy_aircraft(Aircraft* a) {
    if (a != NULL) {

        free(a);
    }
}

char* get_aircraft_identifier(const Aircraft* a) { 
    if (a == NULL) return NULL;
    return strdup(a->identifier); 
}

char* get_aircraft_manufacturer(const Aircraft* a) {
    if (a == NULL) return NULL;
    return strdup(a->manufacturer);
}

char* get_aircraft_model(const Aircraft* a) {
    if (a == NULL) return NULL;
    return strdup(a->model);
}