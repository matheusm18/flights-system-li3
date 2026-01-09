#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>

struct airport {
    char code[4];
    const char* name;
    const char* city;
    const char* country;
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
    
    strncpy(airport->code, code, 3);
    airport->code[3] = '\0';
    airport->name = name;
    airport->city = city;
    airport->country = country;
    airport->type = airport_type_to_char(type);

    return airport;
}

void destroy_airport(Airport* a) {
    if (a != NULL) {

        free(a);
    }
}

char* get_airport_code(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->code);
}

char* get_airport_name(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->name);
}

char* get_airport_city(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->city);
}

char* get_airport_country(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->country);
}

char* get_airport_type(const Airport* airport) {
    if (airport->type == 'S') return strdup("small_airport");
    else if (airport->type == 'M') return strdup("medium_airport");
    else if (airport->type == 'L') return strdup("large_airport");
    else if (airport->type == 'H') return strdup("heliport");
    else if (airport->type == 'P') return strdup("seaplane_base");
    else return strdup("unknown");
}

char airport_type_to_char(const char* type_str) {
    if (strcmp(type_str, "small_airport") == 0) return 'S';
    else if (strcmp(type_str, "medium_airport") == 0) return 'M';
    else if (strcmp(type_str, "large_airport") == 0) return 'L';
    else if (strcmp(type_str, "heliport") == 0) return 'H';
    else if (strcmp(type_str, "seaplane_base") == 0) return 'P';
    else return 'U'; // unknown
}
