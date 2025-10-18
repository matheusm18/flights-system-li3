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

void process_valid_line_flights(char **fields, int num_fields) {
    char *flight_id = fields[0];
    char *departure = fields[1];
    char *actual_departure = fields[2];
    char *arrival = fields[3];
    char *actual_arrival = fields[4];
    char *gate = fields[5];
    char *status = fields[6];
    char *origin = fields[7];
    char *destination = fields[8];
    char *aircraft = fields[9];
    char *airline = fields[10];
    char *tracking_url = fields[11];

    if (!validate_flight_id_flight(flight_id)) {
        printf("Voo descartado: flight_id '%s' inválido\n", flight_id);
        return;
    }
    if (!validate_arrivals_and_departures_flight(departure)) {
        printf("Voo descartado: departure '%s' inválido\n", departure);
        return;
    }
    if (!validate_actual_arrivals_and_departures_flight(departure, actual_departure)) {
        printf("Voo descartado: actual departure '%s' inválido\n", actual_departure);
        return;
    }
    if (!validate_arrivals_and_departures_flight(arrival)) {
        printf("Voo descartado: arrival '%s' inválido\n", arrival);
        return;
    }
    if (!validate_actual_arrivals_and_departures_flight(arrival, actual_arrival)) {
        printf("Voo descartado: actual arrival '%s' inválido\n", actual_arrival);
        return;
    }
    if (!validate_gate_flight(gate)) {
        printf("Voo descartado: gate '%s' inválido\n", gate);
        return;
    }
    if (!validate_status_flight(status, departure, arrival,actual_departure, actual_arrival)) {
        printf("Voo descartado: status '%s' inválido\n", status);
        return;
    }
    if (!validate_origin_flight(origin)) {
        printf("Voo descartado: origin '%s' inválido\n", origin);
        return;
    }
    if (!validate_destination_flight(origin, destination)) {
        printf("Voo descartado: destination '%s' inválido\n", destination);
        return;
    }
    if (!validate_aircraft_flight(aircraft)) {
        printf("Voo descartado: aircraft '%s' inválido\n", aircraft);
        return;
    }
    if (!validate_airline_flight(airline)) {
        printf("Voo descartado: airline '%s' inválido\n", airline);
        return;
    }
    if (!validate_tracking_url_flight(tracking_url, flight_id)) {
        printf("Voo descartado: tracking url '%s' inválido\n", tracking_url);
        return;
    }
}