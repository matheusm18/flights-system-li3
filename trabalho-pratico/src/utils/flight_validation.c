#include "utils/flight_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils/date.h"
#include "utils/date_validation.h"
#include "utils/airport_validation.h"
#include "utils/aircraft_validation.h"

//======== Syntactic validation: Flights

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

//======== Logical validation: Flights

bool validate_destination_origin(const char *origin, const char *destination) {
    if (!origin || !destination) return false;
    
    if (!validate_code_airport(origin) || !validate_code_airport(destination)) return false;
    return (strcmp(origin, destination) != 0);
}

// valida se arrival >= departure e actual_arrival >= actual_departure
bool validate_temporal_consistency(const char *status, long departure, long arrival, long actual_departure, long actual_arrival) {
    // se o status for Cancelled, não comparamos as datas
    if (status && strcmp(status, "Cancelled") == 0) return true; // considera válido

    // arrival não pode ser anterior a departure
    if (compare_datetimes(arrival, departure) < 0) return false;
    
    // actual_arrival não pode ser anterior a actual_departure
    if (compare_datetimes(actual_arrival, actual_departure) < 0) return false;
    
    return true;
}

bool validate_cancelled_status(const char *status, const char *actual_departure_str, const char *actual_arrival_str) {
    if (!status || strcmp(status, "Cancelled") != 0) return true; // só valida se for Cancelled
    
    return (actual_departure_str && actual_arrival_str && strcmp(actual_departure_str, "N/A") == 0 && strcmp(actual_arrival_str, "N/A") == 0);
}

// valida status "Delayed" (actual não pode ser anterior a scheduled)
bool validate_delayed_status(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival) {
    if (!status || strcmp(status, "Delayed") != 0) return true; // só valida se for Delayed
    
    // actual_departure não pode ser anterior a scheduled_departure
    // compare_datetimes retorna <0 quando a primeira data é menor que a segunda
    if (compare_datetimes(actual_departure, scheduled_departure) < 0) return false;
    
    // actual_arrival não pode ser anterior a scheduled_arrival  
    if (compare_datetimes(actual_arrival, scheduled_arrival) < 0) return false;
    
    return true;
}

// validação completa de status
bool validate_status_flight_new(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival,
                                const char *actual_departure_str, const char *actual_arrival_str) {

    if (!status) return false;

    if (strcmp(status, "On Time") == 0) {
        return true;
    }

    else if (strcmp(status, "Delayed") == 0) {
        return validate_delayed_status(status, scheduled_departure, scheduled_arrival, actual_departure, actual_arrival);
    }

    else if (strcmp(status, "Cancelled") == 0) {
        return validate_cancelled_status(status, actual_departure_str, actual_arrival_str);
    }

    return false;
}

bool validate_aircraft_flight(const char *aircraft, AircraftCatalog* aircraft_catalog) {
    return (get_aircraft_by_identifier(aircraft_catalog, aircraft) != NULL);
}

// Validação completa de um voo (lógica)
bool validate_flight_logical(const char *origin, const char *destination, long departure, long arrival, long actual_departure, long actual_arrival,
                             const char *status, const char *aircraft, AircraftCatalog* aircraft_catalog, const char *actual_departure_str,
                             const char *actual_arrival_str) {
    
    // validar se origin e destination são diferentes
    if (!validate_destination_origin(origin, destination)) return false;
    
    // validar se arrival >= departure e actual_arrival >= actual_departure
    if (!validate_temporal_consistency(status, departure, arrival, actual_departure, actual_arrival)) return false;
    
    // verificar se aircraft existe no catálogo
    if (!validate_aircraft_flight(aircraft, aircraft_catalog)) return false;
    
    // validar status
    if (!validate_status_flight_new(status, departure, arrival, actual_departure, actual_arrival, actual_departure_str, actual_arrival_str)) return false;
    
    return true;
}