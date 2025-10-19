#include "utils/airport_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//======== Syntactic validation: Airports

bool validate_code_airport(const char *code) {
    if (!code || strlen(code) != 3) return false;
    for (int i = 0; i < 3; i++) {
        if (code[i] < 'A' || code[i] > 'Z') return false;
    }
    return true;
}

bool validate_name_city_airport(const char *name) {
    if (!name) return false;
    for (int i = 0; name[i]; i++) {
        unsigned char c = (unsigned char) name[i];
        if (c < 32) return false;
    }
    return true;
}

bool validate_country_airport(const char *country) {
    if (!country || strlen(country) != 2) return false;
    for (int i = 0; i < 2; i++) {
        if (country[i] < 'A' || country[i] > 'Z') return false;
    }
    return true;
}

bool validate_latitude_airport(double latitude) {
    return latitude >= -90 && latitude <= 90;
}

bool validate_longitude_airport(double longitude) {
    return longitude >= -180 && longitude <= 180;
}

bool validate_icao_airport(const char *icao) {
    if (!icao || strlen(icao) != 4) return false;
    for (int i = 0; i < 4; i++) {
        if (icao[i] < 'A' || icao[i] > 'Z') return false;
    }
    return true;
}

bool validate_type_airport(const char *type) {
    if (!type) return false;
    return (strcmp(type, "small_airport") == 0 ||
            strcmp(type, "medium_airport") == 0 ||
            strcmp(type, "large_airport") == 0 ||
            strcmp(type, "heliport") == 0 ||
            strcmp(type, "seaplane_base") == 0);
}