#include "utils/validation.h"

#include "utils/aircraft_validation.h"
#include "utils/airport_validation.h"
#include "utils/date_validation.h"
#include "utils/flight_validation.h"
#include "utils/passenger_validation.h"
#include "utils/reservation_validation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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