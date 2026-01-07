#include "validation/entities_validation.h"

#include "catalog/catalog_manager.h"
#include "catalog/airport_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/flight_catalog.h"
#include "validation/aircraft_validation.h"
#include "validation/airport_validation.h"
#include "validation/flight_validation.h"
#include "validation/passenger_validation.h"
#include "validation/reservation_validation.h"
#include "utils/validation_utils.h"
#include "utils/query4_utils.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void init_airports_errors_file() {
    const char *output_path = "resultados/airports_errors.csv";
    FILE *f = fopen(output_path, "w");  
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
    AirportCatalog* airport_catalog = get_airports_from_catalog_manager(manager);

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
    
    char* pooled_name = catalog_manager_get_string_from_pool(manager, name);
    char* pooled_city = catalog_manager_get_string_from_pool(manager, city);
    char* pooled_country = catalog_manager_get_string_from_pool(manager, country);

    Airport* airport = create_airport(code, pooled_name, pooled_city, pooled_country, latitude, longitude, icao, type);
    
    if (airport != NULL) {
        airport_catalog_add(airport_catalog, airport);
    }
}

void init_aircrafts_errors_file() {
    const char *output_path = "resultados/aircrafts_errors.csv";
    FILE *f = fopen(output_path, "w"); 
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
    AircraftCatalog* aircraft_catalog = get_aircrafts_from_catalog_manager(manager);

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

    char* pooled_manufacturer = catalog_manager_get_string_from_pool(manager, manufacturer);
    char* pooled_model = catalog_manager_get_string_from_pool(manager, model);

    Aircraft* aircraft = create_aircraft(identifier, pooled_manufacturer, pooled_model, year_int, capacity_int, range_int);

    if (aircraft != NULL) {
        aircraft_catalog_add(aircraft_catalog, aircraft);
    }
}

void init_flights_errors_file() {
    const char *output_path = "resultados/flights_errors.csv";
    FILE *f = fopen(output_path, "w");  
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
    FlightCatalog* flight_catalog = get_flights_from_catalog_manager(manager);
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

    long departure_dt = string_to_datetime(departure);
    long actual_departure_dt = string_to_datetime(actual_departure);
    long arrival_dt = string_to_datetime(arrival);
    long actual_arrival_dt = string_to_datetime(actual_arrival);

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

    char* pooled_status = catalog_manager_get_string_from_pool(manager, status);
    char* pooled_aircraft = catalog_manager_get_string_from_pool(manager, aircraft);

    Flight* flight = create_flight(flight_id, departure_dt, actual_departure_dt, arrival_dt, actual_arrival_dt, gate, pooled_status, origin, destination, 
                                   pooled_aircraft, airline);        

    if (flight != NULL) {
        flight_catalog_add(flight_catalog, flight);

        if (!is_canceled(status)) {
            aircrafts_counter_increment(aircraft, aircraft_catalog);

            char* origin_code = get_flight_origin(flight);
            airport_catalog_add_flight(airport_catalog, origin, flight);
            free(origin_code);
        }
        if (is_delayed(status)) {
            int delay = get_flight_delay(actual_departure_dt,departure_dt);
            AirlineStats* stats = get_airline_stats_by_identifier(flight_catalog, airline);

            if (stats == NULL) {
                flight_catalog_add_airline_stats(flight_catalog, airline);
                stats = get_airline_stats_by_identifier(flight_catalog, airline);
            }
            
            airline_stats_increment(flight_catalog, airline, delay);
        }
    }
}

void init_passengers_errors_file() {
    const char *output_path = "resultados/passengers_errors.csv";
    FILE *f = fopen(output_path, "w"); 
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
    PassengerCatalog* passenger_catalog = get_passengers_from_catalog_manager(manager);

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

    int dob_int = string_to_date(dob);

    char* pooled_first_name = catalog_manager_get_string_from_pool(manager, first_name);
    char* pooled_last_name = catalog_manager_get_string_from_pool(manager, last_name);
    char* pooled_nationality = catalog_manager_get_string_from_pool(manager, nationality);

    Passenger* passenger = create_passenger(document_number, pooled_first_name, pooled_last_name, dob_int, pooled_nationality);
    
    if (passenger != NULL) passenger_catalog_add(passenger_catalog, passenger);
}

void init_reservations_errors_file() {
    const char *output_path = "resultados/reservations_errors.csv";
    FILE *f = fopen(output_path, "w");  
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
    AirportCatalog* airport_catalog = get_airports_from_catalog_manager(manager);
    FlightCatalog* flight_catalog = get_flights_from_catalog_manager(manager);
    PassengerCatalog* passenger_catalog = get_passengers_from_catalog_manager(manager);
    ReservationCatalog* reservation_catalog = get_reservations_from_catalog_manager(manager);

    char *reservation_id    = fields[0];
    char *flight_ids        = fields[1];
    char *document_number   = fields[2];
    char *seat              = fields[3];
    char *price             = fields[4];
    char *extra_luggage     = fields[5];
    char *priority_boarding = fields[6];
    char *qr_code           = fields[7];

    if (!validate_reservation_id(reservation_id) ||
        !validate_flight_ids_reservation(flight_ids, flight_catalog) ||
        !validate_passenger_document_number(document_number) ||
        !validate_document_number_reservation(document_number, passenger_catalog)) {

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

    char *clean_ids = strdup(flight_ids);
    if (!clean_ids) return;

    char *delims = "[]' ,\"";

    int flight_count = 0;
    char *token = strtok(clean_ids, delims);

    double total_price = atof(price);

    char week_buf[20] = {0}; // buffer de stack válido durante toda a função
    char* q4_week_id = NULL;

    while (token && flight_count < 2) {

        Flight* flight =
            get_flight_by_flight_id_from_catalog(flight_catalog, token);

        if (flight) {
            char* status = get_flight_status(flight);

            if (status && !is_canceled(status)) {
                char* origin = get_flight_origin(flight);
                char* destination = get_flight_destination(flight);

                if (origin)
                    airport_passenger_increment(airport_catalog, origin, 'd');
                if (destination)
                    airport_passenger_increment(airport_catalog, destination, 'a');

                Passenger* p = get_passenger_by_dnumber(passenger_catalog, atoi(document_number));

                if (p && destination) {
                    char* nationality = get_passenger_nationality(p);

                    if (nationality) {
                        reservation_catalog_add_nationality_increment(reservation_catalog, nationality, destination);
                    }

                    free(nationality);
                }

                if (origin) free(origin);
                if (destination) free(destination);

            }

            if (status) free(status);

            if (flight_count == 0) {
                    long departure_date = get_date_part(get_flight_departure(flight));
                    if (departure_date) {
                        date_to_week_key_buf(departure_date, week_buf);
                        q4_week_id = week_buf;

                    }
            }
        }
        
        flight_count++;
        token = strtok(NULL, delims);
    }

    free(clean_ids);

    if (q4_week_id != NULL && q4_week_id[0] != '\0') {
        passenger_catalog_add_price_increment(passenger_catalog, q4_week_id, document_number, total_price);
    }

    /* Decidimos não armazenar as reservas visto que não precisavamos destes dados

    bool extra_luggage_bool = string_to_bool(extra_luggage);
    bool priority_boarding_bool =
        string_to_bool(priority_boarding);

    char* pooled_reservation_id = catalog_manager_get_string_from_pool(manager, reservation_id);
    char* pooled_document_number = catalog_manager_get_string_from_pool(manager, document_number);
    char* pooled_seat = catalog_manager_get_string_from_pool(manager, seat);

    Reservation* res =
        create_reservation(pooled_reservation_id,
                           flight_ids_array,
                           pooled_document_number,
                           pooled_seat,
                           atof(price),
                           extra_luggage_bool,
                           priority_boarding_bool);

    if (res)
        reservation_catalog_add(reservation_catalog, res);
    */
}
