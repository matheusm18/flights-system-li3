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

bool validate_decimal_cases(const char *str) {
    if (!str) return false;

    const char *p = str;
    if (*p == '-') p++;  

    if (*p == '\0') return false; 

    bool dot_found = false;
    int decimals = 0;

    for (; *p != '\0'; p++) {
        if (*p == '.') {
            if (dot_found) return false;  
            dot_found = true;
            continue; 
        }

        if (!isdigit((unsigned char) *p)) return false;

        if (dot_found) decimals++;
        if (decimals > 8) return false;
    }

    return true;
}

bool validate_latitude_airport(const char *latitude) {
    if (!latitude) return false;

    if (!validate_decimal_cases(latitude))
        return false;

    char *invalid_start;
    double value = strtod(latitude, &invalid_start); ///< Converte para double

    
    if (*invalid_start != '\0') return false; ///< Verifica se sobrou "lixo" no fim da conversão

    if (value < -90.0 || value > 90.0)
        return false;

    return true;
}

bool validate_longitude_airport(const char *longitude) {
    if (!longitude) return false;

    if (!validate_decimal_cases(longitude))
        return false;

    char *invalid_start;
    double value = strtod(longitude, &invalid_start); 

    if (*invalid_start != '\0') return false;

    if (value < -180.0 || value > 180.0)
        return false;

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