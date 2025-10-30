#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>

struct airport {
    char* code;
    char* name;
    char* city;
    char* country;
    double latitude;
    double longitude;
    char* icao;
    char* type;
};

Airport* create_airport(const char* code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type) {

    Airport* airport = malloc(sizeof(Airport));
    if (airport == NULL) return NULL;

    airport->code = strdup(code);
    airport->name = strdup(name);
    airport->city = strdup(city);
    airport->country = strdup(country);
    airport->latitude = atof(latitude);
    airport->longitude = atof(longitude);
    airport->icao = strdup(icao);
    airport->type = strdup(type);
    
    return airport;
}

void destroy_airport(Airport* a) {
    if (a != NULL) {

        free(a->code);
        free(a->name);
        free(a->city);
        free(a->country);
        free(a->icao);
        free(a->type);

        free(a);
    }
}

const char* get_airport_code(const Airport* airport) {
    if (airport == NULL) return NULL;
    return airport->code;
}

const char* get_airport_name(const Airport* airport) {
    if (airport == NULL) return NULL;
    return airport->name;
}

const char* get_airport_city(const Airport* airport) {
    if (airport == NULL) return NULL;
    return airport->city;
}

const char* get_airport_country(const Airport* airport) {
    if (airport == NULL) return NULL;
    return airport->country;
}

const char* get_airport_type(const Airport* airport) {
    if (airport == NULL) return NULL;
    return airport->type;
}