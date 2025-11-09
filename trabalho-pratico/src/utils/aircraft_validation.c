#include "utils/aircraft_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//======== Syntactic validation: Aircrafts

bool validate_year_aircraft(const char *string) {
    if (!string) return false;

    if (strlen(string) != 4) return false;

    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) return false;
    }

    int year = atoi(string);

    if (year > 2025) return false;

    return true;
}