#include "utils/flight_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


bool validate_flight_id_flight(const char *flight_id) {
    if (!flight_id || strlen(flight_id) != 7) return false;
    for (int i = 0; i < 2; i++) {
        if (flight_id[i] < 'A' || flight_id[i] > 'Z') return false;
    }
    for (int i = 2; i < 7; i++) {
        if (flight_id[i] < '0' || flight_id[i] > '9') return false;
    }
    return true;
}