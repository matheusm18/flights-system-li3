#include "utils/validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//======== Syntactic validation: Dates

bool validate_date(char* date) {
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7) {
            if(date[i] < '0' || date[i] > '9') return false;
        }
    }

    int year, month, day;
    sscanf(date,"%d-%d-%d",&year, &month, &day);

    return (month >= 1 && month <= 12 && day >= 1 && day <= 31);

}

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
        unsigned char c = (unsigned char)name[i];

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


bool valida_longitude_airport(double longitude) {
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

void process_valid_line(char **fields, int num_fields) {
    char *code = fields[0];
    char *name = fields[1];
    char *city = fields[2];
    char *country = fields[3];
    double latitude = atof(fields[4]);
    double longitude = atof(fields[5]);
    char *icao = fields[6];
    char *type = fields[7];

    if (!validate_code_airport(code)) {
        printf("Aeroporto descartado: código '%s' inválido\n", code);
        return;
    }
    if (!validate_name_city_airport(name)) {
        printf("Aeroporto descartado: nome '%s' inválido\n", name);
        return;
    }
    if (!validate_name_city_airport(city)) {
        printf("Aeroporto descartado: cidade '%s' inválida\n", city);
        return;
    }
    if (!validate_country_airport(country)) {
        printf("Aeroporto descartado: país '%s' inválido\n", country);
        return;
    }
    if (!validate_latitude_airport(latitude)) {
        printf("Aeroporto descartado: latitude '%f' inválida\n", latitude);
        return;
    }
    if (!validate_longitude_airport(longitude)) {
        printf("Aeroporto descartado: longitude '%f' inválida\n", longitude);
        return;
    }
    if (!validate_icao_airport(icao)) {
        printf("Aeroporto descartado: icao '%s' inválida\n", icao);
        return;
    }
    if (!validate_type_airport(type)) {
        printf("Aeroporto descartado: type '%s' inválida\n", type);
        return;
    }
}