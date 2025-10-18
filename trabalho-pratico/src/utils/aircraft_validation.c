#include "utils/aircraft_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//======== Syntactic validation: Aircrafts

bool validate_identifier_aircraft(const char *identifier) {
    if (!identifier || strlen(identifier) != 8) return false;

    if (identifier[2] != '-') return false;

    for (int i = 0; i < 8; i++) {

        if (i != 2) {
            if (!isupper(identifier[i]) && !isdigit(identifier[i])) return false;
        }
    }
    return true;
}

bool validate_manufacturer_aircraft(const char *manufacturer) {
    if (!manufacturer) return false;

    if (!isupper(manufacturer[0])) return false;

    for(int i = 1; manufacturer[i] != '\0'; i++) {
        if (!isalpha(manufacturer[i])) return false;
    }
    return true;
}

bool validate_model_aircraft(const char *model) {
    if (!model) return false;

    for (int i = 0; model[i] != '\0'; i++) {
        unsigned char c = (unsigned char) model[i];
        if (c < 32 || c > 126) return false;
    }

    return true;
}

bool validate_year_aircraft(const char *string) {
    if (!string) return false;

    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) return false;
    }

    int year = atoi(string);

    if (year < 1900 || year > 2025) return false; // defini 1900 como ano minimo que faça sentido ter voos

    return true;
}

bool validate_capacity_aircraft(const char *string) {
    if (!string) return false;

    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) return false;
    }

    int capacity = atoi(string);

    if (capacity <= 0) return false;

    return true;
}

bool validate_range_aircraft(const char *string) {
    if (!string) return false;

    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) return false;
    }

    int range = atoi(string);

    if (range <= 0) return false;

    return true;
}