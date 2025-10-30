#include "utils/validation.h"

#include "catalog/airport_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "catalog/flight_catalog.h"
#include "utils/aircraft_validation.h"
#include "utils/airport_validation.h"
#include "utils/date_validation.h"
#include "utils/flight_validation.h"
#include "utils/passenger_validation.h"
#include "utils/reservation_validation.h"
#include "utils/date.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void process_valid_line_aircrafts(char **fields, int num_fields, void* catalog) {
    AircraftCatalog* aircraft_catalog = (AircraftCatalog*)catalog;

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

    int year_int = atoi(year);
    int capacity_int = atoi(capacity);
    int range_int = atoi(range);

    Aircraft* aircraft = create_aircraft(identifier, manufacturer, model, year_int, capacity_int, range_int);

    if (aircraft != NULL) {
        aircraft_catalog_add(aircraft_catalog, aircraft);
    }
    
}

void process_valid_line_airports(char **fields, int num_fields, void* catalog) {
    AirportCatalog* airport_catalog = (AirportCatalog*)catalog;

    char *code = fields[0];
    char *name = fields[1];
    char *city = fields[2];
    char *country = fields[3];
    char *latitude = fields[4];
    char *longitude = fields[5];
    char *icao = fields[6];
    char *type = fields[7];

    if (!validate_code_airport(code) || !validate_name_city_airport(name) || !validate_name_city_airport(city) || !validate_country_airport(country) || !validate_latitude_airport(latitude) ||
        !validate_longitude_airport(longitude) || !validate_icao_airport(icao) || !validate_type_airport(type)) {
        
        // será preciso escrever caso haja erro? ex: indicar os erros? ver melhor
        return;
    }

    Airport* airport = create_airport(code, name, city, country, latitude, longitude, icao, type);
    
    if (airport != NULL) {
        airport_catalog_add(airport_catalog, airport);
    }
}

void process_valid_line_flights(char **fields, int num_fields, void* catalog) {
    FlightCatalog* flight_catalog = (FlightCatalog*)catalog;

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

    if (!validate_flight_id_flight(flight_id) || !validate_arrivals_and_departures_flight(departure) || !validate_actual_arrivals_and_departures_flight(departure, actual_departure) ||
        !validate_arrivals_and_departures_flight(arrival) || !validate_actual_arrivals_and_departures_flight(arrival, actual_arrival) || !validate_gate_flight(gate) ||
        !validate_status_flight(status, departure, arrival,actual_departure, actual_arrival) || !validate_origin_flight(origin) || !validate_destination_flight(origin, destination) ||
        !validate_aircraft_flight(aircraft) || !validate_airline_flight(airline) || !validate_tracking_url_flight(tracking_url, flight_id)) {
            return;
        }
        Flight* flight = create_flight(flight_id,
            datetime_create_from_string(departure),
            datetime_create_from_string(actual_departure),
            datetime_create_from_string(arrival),
            datetime_create_from_string(actual_arrival),
            gate, status, origin, destination, aircraft, airline, tracking_url);        
    
        if (flight != NULL) {
            flight_catalog_add(flight_catalog, flight);
        }
}

void process_valid_line_passengers(char **fields, int num_fields, void* catalog) {
    char *document_number = fields[0];
    char *first_name = fields[1];
    char *last_name = fields[2];
    char *dob = fields[3];
    //char *nationality = fields[4];
    char *gender = fields[5];
    char *email = fields[6];
    //char *phone = fields[7];
    //char *adress = fields[8];
    //char *photo = fields[9];

    if (!validate_passenger_document_number(document_number)) {
        printf("Passageiro descartado: número de documento '%s' inválido!\n", document_number);
        return;
    }

    if(!validate_passenger_email(email)){
        printf("Passageiro descartado: email '%s' inválido!\n", email);
        return;
    }

    if(!validate_passenger_gender(gender)){
        printf("Passageiro descartado: género '%s' inválido!\n", gender);
        return;
    }

    if(!validate_passenger_birth_date(dob)){
        printf("Passageiro descartado: data de nascimento '%s' inválida!\n", dob);
        return;
    }

    if(!validate_passenger_name(first_name)){
        printf("Passageiro descartado: nome '%s' inválido!\n", first_name);
        return;

    }

    if(!validate_passenger_name(last_name)){
        printf("Passageiro descartado: nome '%s' inválido!\n", last_name);
        return;

    }
}

void process_valid_line_reservations(char **fields, int num_fields, void* catalog) {
    char *reservation_id = fields[0];
    char *flight_ids = fields[1];
    char *document_number = fields[2];
    char *seat = fields[3];
    char *price = fields[4];
    char *extra_luggage = fields[5];
    char *priority_boarding = fields[6];
    char *qr_code = fields[7];

    if (!validate_reservation_id(reservation_id)) {
        printf("Reserva descartada: id de reserva '%s' inválido!\n", reservation_id);
        return;
    }

    if (!validate_flight_ids_reservation(flight_ids)) {
        printf("Reserva descartada: ids de voos '%s' inválidos!\n", flight_ids);
        return;
    }

    if (!validate_document_number_reservation(document_number)) {
        printf("Reserva descartada: número de documento '%s' inválido!\n", document_number);
        return;
    }

    if (!validate_seat_reservation(seat)) {
        printf("Reserva descartada: assento '%s' inválido!\n", seat);
        return;
    }

    if (!validate_price_reservation(price)) {
        printf("Reserva descartada: preço '%s' inválido!\n", price);
        return;
    }

    if (!validate_extra_luggage_reservation(extra_luggage)) {
        printf("Reserva descartada: bagagem extra '%s' inválida!\n", extra_luggage);
        return;
    }

    if (!validate_priority_boarding_reservation(priority_boarding)) {
        printf("Reserva descartada: embarque prioritário '%s' inválido!\n", priority_boarding);
        return;
    }

    if (!validate_qr_code_reservation(qr_code)) {
        printf("Reserva descartada: qr code '%s' inválido!\n", qr_code);
        return;
    }
}