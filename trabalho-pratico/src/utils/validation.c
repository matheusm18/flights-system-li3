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

void process_valid_line_aircrafts(char **fields, int num_fields) {
    char *identifier = fields[0];
    char *manufacturer = fields[1];
    char *model = fields[2];
    char *year = fields[3];
    char *capacity = fields[4];
    char *range = fields[5];

    if (!validate_identifier_aircraft(identifier)) {
        printf("Aeronave descartada: identificador '%s' inválido\n", identifier);
        return;
    }
    if (!validate_manufacturer_aircraft(manufacturer)) {
        printf("Aeronave descartada: fabricante '%s' inválido\n", manufacturer);
        return;
    }
    if (!validate_model_aircraft(model)) {
        printf("Aeronave descartada: modelo '%s' inválido\n", model);
        return;
    }
    if (!validate_year_aircraft(year)) {
        printf("Aeronave descartada: ano '%s' inválido\n", year);
        return;
    }
    if (!validate_capacity_aircraft(capacity)) {
        printf("Aeronave descartada: capacidade '%s' inválida\n", capacity);
        return;
    }
    if (!validate_range_aircraft(range)) {
        printf("Aeronave descartada: alcance '%s' inválido\n", range);
        return;
    }
}

void process_valid_line_airports(char **fields, int num_fields) {
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