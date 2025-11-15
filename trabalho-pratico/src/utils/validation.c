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

void process_valid_line_aircrafts(char **fields, int num_fields, void* user_data, FILE *errors_file) {
    (void) num_fields;
    CatalogManager* manager = (CatalogManager*) user_data;

    char *identifier = fields[0];
    char *manufacturer = fields[1];
    char *model = fields[2];
    char *year = fields[3];
    char *capacity = fields[4];
    char *range = fields[5];

    if (!validate_year_aircraft(year)) {

        fprintf(errors_file,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                identifier,
                manufacturer,
                model,
                year,
                capacity,
                range);
        return;
    }

    int year_int = atoi(year);
    int capacity_int = atoi(capacity);
    int range_int = atoi(range);

    Aircraft* aircraft = create_aircraft(identifier, manufacturer, model, year_int, capacity_int, range_int);

    if (aircraft != NULL) {
        aircraft_catalog_add(get_aircrafts_from_catalog_manager(manager), aircraft);
    }
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

void process_valid_line_airports(char **fields, int num_fields, void* user_data, FILE *errors_file) {
    (void) num_fields;
    CatalogManager* manager = (CatalogManager*) user_data;

    char *code = fields[0];
    char *name = fields[1];
    char *city = fields[2];
    char *country = fields[3];
    char *latitude = fields[4];
    char *longitude = fields[5];
    char *icao = fields[6];
    char *type = fields[7];

    if (!validate_code_airport(code) || !validate_latitude_airport(latitude) ||
        !validate_longitude_airport(longitude) || !validate_type_airport(type)) {

        fprintf(errors_file,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                code,
                name,
                city,
                country,
                latitude,
                longitude,
                icao,
                type);
        return;
    }

    Airport* airport = create_airport(code, name, city, country, latitude, longitude, icao, type);
    
    if (airport != NULL) {
        airport_catalog_add(get_airports_from_catalog_manager(manager), airport);
    }
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

void process_valid_line_flights(char **fields, int num_fields, void* user_data, FILE *errors_file) {
    (void) num_fields;
    CatalogManager* manager = (CatalogManager*) user_data;
    AircraftCatalog* aircraft_catalog = get_aircrafts_from_catalog_manager(manager);
    AirportCatalog* airport_catalog = get_airports_from_catalog_manager(manager);

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

    // validações sintática
    if (!validate_flight_id_flight(flight_id) || !validate_datetime(departure) || !validate_datetime(actual_departure) ||
        !validate_datetime(arrival) || !validate_datetime(actual_arrival)) {

        fprintf(errors_file,
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
        return;
    }

    // vamos tratar os casos dos campos com 'N/A' convertendo para -1 no datetime

    long departure_dt = string_to_datetime(departure);
    long actual_departure_dt = string_to_datetime(actual_departure);
    long arrival_dt = string_to_datetime(arrival);
    long actual_arrival_dt = string_to_datetime(actual_arrival);

    // validações lógica
    if (!validate_flight_logical(origin, destination, departure_dt, arrival_dt, actual_departure_dt, actual_arrival_dt,
                                 status, aircraft, aircraft_catalog, actual_departure, actual_arrival)) {

        fprintf(errors_file,
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
        return;
    }

    Flight* flight = create_flight(flight_id, departure_dt, actual_departure_dt, arrival_dt, actual_arrival_dt, gate, status, origin, destination, 
                                   aircraft, airline);        

    if (flight != NULL) {
        flight_catalog_add(get_flights_from_catalog_manager(manager), flight);

        if (strcmp(status, "Cancelled") != 0) {
            aircrafts_counter_increment(get_aircraft_id_from_flight(flight), aircraft_catalog);

            const char* origin_code = get_flight_origin(flight); // obter o código do aeroporto de origem
            Airport* airport = get_airport_by_code(airport_catalog, origin_code);

            if (airport != NULL) {
                airport_add_departing_flight(airport, flight); // adicionar o voo à lista de partidas do aeroporto
            }
        }
    }
}

void init_passengers_errors_file() {
    const char *output_path = "resultados/passengers_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro passengers_errors.csv");
        return;
    }
    fprintf(f, "\"document number\",\"first name\",\"last name\",\"dob\",\"nationality\",\"gender\",\"email\",\"phone\",\"address\",\"photo\"\n");
    fclose(f);
}

void process_valid_line_passengers(char **fields, int num_fields, void* user_data, FILE *errors_file) {
    (void) num_fields;
    CatalogManager* manager = (CatalogManager*) user_data;

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
        !validate_passenger_birth_date(dob)) {

        fprintf(errors_file,
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
        return;
    }

    // para esta fase em vez de criar o passageiro e armazenar no catálogo, vamos só armazenar o document number para a validação das reservas
    int document_number_int = atoi(document_number);
    passenger_catalog_add(get_passengers_from_catalog_manager(manager), document_number_int);
    /*
    int dob_dt = string_to_date(dob);

    Passenger* passenger = create_passenger(document_number, first_name, last_name, dob_dt, nationality, gender);
    
    if (passenger != NULL) {
        passenger_catalog_add(get_passengers_from_catalog_manager(manager), passenger);
    }
    */
}

void init_reservations_errors_file() {
    const char *output_path = "resultados/reservations_errors.csv";
    FILE *f = fopen(output_path, "w");  // cria/limpa o ficheiro
    if (f == NULL) {
        perror("Erro ao criar ficheiro reservations_errors.csv");
        return;
    }
    fprintf(f, "\"reservation id\",\"flight ids\",\"document number\",\"seat\",\"price\",\"extra luggage\",\"priority boarding\",\"qr code\"\n");
    fclose(f);
}

void process_valid_line_reservations(char **fields, int num_fields, void* user_data, FILE *errors_file) {
    (void) num_fields;
    CatalogManager* manager = (CatalogManager*) user_data;
    FlightCatalog* flight_catalog = get_flights_from_catalog_manager(manager);
    PassengerCatalog* passenger_catalog = get_passengers_from_catalog_manager(manager);
    
    char *reservation_id = fields[0];
    char *flight_ids = fields[1];
    char *document_number = fields[2];
    char *seat = fields[3];
    char *price = fields[4];
    char *extra_luggage = fields[5];
    char *priority_boarding = fields[6];
    char *qr_code = fields[7];

    if (!validate_reservation_id(reservation_id) ||
        !validate_flight_ids_reservation(flight_ids, flight_catalog) ||
        !validate_passenger_document_number(document_number) || // verifica formato
        !validate_document_number_reservation(document_number, passenger_catalog)) { // verifica existência

        fprintf(errors_file,
                "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                reservation_id,
                flight_ids,
                document_number,
                seat,
                price,
                extra_luggage,
                priority_boarding,
                qr_code);
        return;
    }

    /*
    // conversão flight_ids
    char flight_id1[10];
    char flight_id2[10] = ""; // vazio se não existir
    int num_flights = 1;

    if (strlen(flight_ids) > 12) { // string com 2 voos
        strncpy(flight_id1, flight_ids + 2, 7); // copia ID1
        flight_id1[7] = '\0';
        strncpy(flight_id2, flight_ids + 12, 7); // copia ID2
        flight_id2[7] = '\0';
        num_flights = 2;
    } else {
        strncpy(flight_id1, flight_ids + 2, 7); // copia ID1
        flight_id1[7] = '\0';
    }

    char* flights[2];
    flights[0] = flight_id1;
    flights[1] = (num_flights == 2) ? flight_id2 : NULL;

    Reservation* res = create_reservation(reservation_id, flights, document_number, seat, atof(price), atoi(extra_luggage), atoi(priority_boarding));
    */
}