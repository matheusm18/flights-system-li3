#include "utils/validation.h"

#include "catalog/catalog_manager.h"
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

void init_aircrafts_errors_file() {
    const char *output_path = "resultados/aircrafts_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro aircrafts_errors.csv");
        return;
    }
    fprintf(f, "\"identifier\",\"manufacturer\",\"model\",\"year\",\"capacity\",\"range\"\n");
    fclose(f);
}

void process_valid_line_aircrafts(char **fields, int num_fields, void* user_data) {
    CatalogManager* manager = (CatalogManager*) user_data;

    const char *output_path = "resultados/aircrafts_errors.csv";
    FILE *aircrafts_errors = fopen(output_path, "a");  // abrir em modo append
    if (aircrafts_errors == NULL) {
        perror("Erro ao abrir ficheiro aircrafts_errors.csv");
        return;
    }

    char *identifier = fields[0];
    char *manufacturer = fields[1];
    char *model = fields[2];
    char *year = fields[3];
    char *capacity = fields[4];
    char *range = fields[5];

    if (!validate_identifier_aircraft(identifier) ||
        !validate_manufacturer_aircraft(manufacturer) ||
        !validate_model_aircraft(model) ||
        !validate_year_aircraft(year) ||
        !validate_capacity_aircraft(capacity) ||
        !validate_range_aircraft(range)) {

        fprintf(aircrafts_errors,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                identifier,
                manufacturer,
                model,
                year,
                capacity,
                range);

        fclose(aircrafts_errors);
        return;
    }

    int year_int = atoi(year);
    int capacity_int = atoi(capacity);
    int range_int = atoi(range);

    Aircraft* aircraft = create_aircraft(identifier, manufacturer, model, year_int, capacity_int, range_int);

    if (aircraft != NULL) {
        aircraft_catalog_add(catalog_manager_get_aircrafts(manager), aircraft);
    }

    fclose(aircrafts_errors);
}
void init_airports_errors_file() {
    const char *output_path = "resultados/airports_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro airports_errors.csv");
        return;
    }
    fprintf(f, "\"code\",\"name\",\"city\",\"country\",\"latitude\",\"longitude\",\"icao\",\"type\"\n");
    fclose(f);
}

void process_valid_line_airports(char **fields, int num_fields, void* user_data) {
    CatalogManager* manager = (CatalogManager*) user_data;

    const char *output_path = "resultados/airports_errors.csv";
    FILE *airports_errors = fopen(output_path, "a");  // abrir em modo append
    if (airports_errors == NULL) {
        perror("Erro ao abrir ficheiro airports_errors.csv");
        return;
    }

    char *code = fields[0];
    char *name = fields[1];
    char *city = fields[2];
    char *country = fields[3];
    char *latitude = fields[4];
    char *longitude = fields[5];
    char *icao = fields[6];
    char *type = fields[7];

    if (!validate_code_airport(code) || !validate_name_city_airport(name) || !validate_name_city_airport(city) ||
        !validate_country_airport(country) || !validate_latitude_airport(latitude) ||
        !validate_longitude_airport(longitude) || !validate_icao_airport(icao) || !validate_type_airport(type)) {

        fprintf(airports_errors,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                code,
                name,
                city,
                country,
                latitude,
                longitude,
                icao,
                type);

        fclose(airports_errors);
        return;
    }

    Airport* airport = create_airport(code, name, city, country, latitude, longitude, icao, type);
    
    if (airport != NULL) {
        airport_catalog_add(catalog_manager_get_airports(manager), airport);
    }

    fclose(airports_errors);
}

void init_flights_errors_file() {
    const char *output_path = "resultados/flights_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro flights_errors.csv");
        return;
    }
    fprintf(f, "\"flight id\",\"departure\",\"actual departure\",\"arrival\",\"actual arrival\",\"gate\",\"status\",\"origin\",\"destination\",\"aircraft\",\"airline\",\"tracking url\"\n");
    fclose(f);
}

void process_valid_line_flights(char **fields, int num_fields, void* user_data) {
    CatalogManager* manager = (CatalogManager*) user_data;

    const char *output_path = "resultados/flights_errors.csv";
    FILE *flights_errors = fopen(output_path, "a");  // append
    if (flights_errors == NULL) {
        perror("Erro ao abrir ficheiro flights_errors");
        return;
    }

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
        !validate_status_flight(status, departure, arrival, actual_departure, actual_arrival) || !validate_origin_flight(origin) || !validate_destination_flight(origin, destination) ||
        !validate_aircraft_flight(aircraft) || !validate_airline_flight(airline) || !validate_tracking_url_flight(tracking_url, flight_id)) {

        fprintf(flights_errors,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                flight_id,
                departure,
                actual_departure,
                arrival,
                actual_arrival,
                gate,
                status,
                origin,
                destination,
                aircraft,
                airline,
                tracking_url);

        fclose(flights_errors);
        return;
    }

    Flight* flight = create_flight(flight_id,
        datetime_create_from_string(departure),
        datetime_create_from_string(actual_departure),
        datetime_create_from_string(arrival),
        datetime_create_from_string(actual_arrival),
        gate, status, origin, destination, aircraft, airline, tracking_url);        

    if (flight != NULL) {
        flight_catalog_add(catalog_manager_get_flights(manager), flight);
    }

    fclose(flights_errors);
}


void init_passengers_errors_file() {
    const char *output_path = "resultados/passengers_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro passengers_errors.csv");
        return;
    }
    fprintf(f, "\"document_number\",\"first_name\",\"last_name\",\"dob\",\"nationality\",\"gender\",\"email\",\"phone\",\"address\",\"photo\"\n");
    fclose(f);
}

void process_valid_line_passengers(char **fields, int num_fields, void* user_data) {
    //CatalogManager* manager = (CatalogManager*) user_data;

    const char *output_path = "resultados/passengers_errors.csv";
    FILE *passengers_errors = fopen(output_path, "a");  // abrir em modo append
    if (passengers_errors == NULL) {
        perror("Erro ao abrir ficheiro passengers_errors.csv");
        return;
    }

    char *document_number = fields[0];
    char *first_name = fields[1];
    char *last_name = fields[2];
    char *dob = fields[3];
    char *nationality = fields[4];
    char *gender = fields[5];
    char *email = fields[6];
    char *phone = fields[7];
    char *address = fields[8];
    char *photo = fields[9];

    if (!validate_passenger_document_number(document_number) ||
        !validate_passenger_email(email) ||
        !validate_passenger_gender(gender) ||
        !validate_passenger_birth_date(dob) ||
        !validate_passenger_name(first_name) ||
        !validate_passenger_name(last_name)) {

        fprintf(passengers_errors,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                document_number,
                first_name,
                last_name,
                dob,
                nationality,
                gender,
                email,
                phone,
                address,
                photo);
    }

    fclose(passengers_errors);
}

void init_reservations_errors_file() {
    const char *output_path = "resultados/reservations_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro reservations_errors.csv");
        return;
    }
    fprintf(f, "\"reservation_id\",\"flight_ids\",\"document_number\",\"seat\",\"price\",\"extra_luggage\",\"priority_boarding\",\"qr_code\"\n");
    fclose(f);
}

void process_valid_line_reservations(char **fields, int num_fields, void* user_data) {
    //CatalogManager* manager = (CatalogManager*) user_data;
    
    const char *output_path = "resultados/reservations_errors.csv";
    FILE *reservations_errors = fopen(output_path, "a");  // abrir em modo append
    if (reservations_errors == NULL) {
        perror("Erro ao abrir ficheiro reservations_errors.csv");
        return;
    }

    char *reservation_id = fields[0];
    char *flight_ids = fields[1];
    char *document_number = fields[2];
    char *seat = fields[3];
    char *price = fields[4];
    char *extra_luggage = fields[5];
    char *priority_boarding = fields[6];
    char *qr_code = fields[7];

    if (!validate_reservation_id(reservation_id) ||
        !validate_flight_ids_reservation(flight_ids) ||
        !validate_document_number_reservation(document_number) ||
        !validate_seat_reservation(seat) ||
        !validate_price_reservation(price) ||
        !validate_extra_luggage_reservation(extra_luggage) ||
        !validate_priority_boarding_reservation(priority_boarding) ||
        !validate_qr_code_reservation(qr_code)) {

        fprintf(reservations_errors,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                reservation_id,
                flight_ids,
                document_number,
                seat,
                price,
                extra_luggage,
                priority_boarding,
                qr_code);
    }

    fclose(reservations_errors);
}
