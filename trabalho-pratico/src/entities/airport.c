#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>

struct airport {
    char code[4];
    char* name;
    char* city;
    char* country;
    //double latitude;
    //double longitude;
    //char icao[5];
    char type;
};

Airport* create_airport(const char *code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type) {
    (void) latitude;
    (void) longitude;
    (void) icao;
    
    Airport* airport = malloc(sizeof(Airport));
    if (airport == NULL) return NULL;
    
    strncpy(airport->code, code, 4);
    airport->code[3] = '\0';
    airport->name = strdup(name);
    airport->city = strdup(city);
    airport->country = strdup(country);
    //airport->latitude = atof(latitude);
    //airport->longitude = atof(longitude);
    //strncpy(airport->icao, icao, 5);
    airport->type = get_airport_type_char(type);
    
    return airport;
}

void destroy_airport(Airport* a) {
    if (a != NULL) {

        free(a->name);
        free(a->city);
        free(a->country);

        free(a);
    }
}

const char* airport_type_to_string(char type) {
    if (type == 'S') return "small_airport";
    else if (type == 'M') return "medium_airport";
    else if (type == 'L') return "large_airport";
    else return "unknown";
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

char get_airport_type(const Airport* airport) {
    if (airport == NULL) return '\0';
    return airport->type;
}

char get_airport_type_char(const char* type_str) {
    if (strcmp(type_str, "small_airport") == 0) return 'S';
    else if (strcmp(type_str, "medium_airport") == 0) return 'M';
    else if (strcmp(type_str, "large_airport") == 0) return 'L';
    else return 'U'; // unknown
}
