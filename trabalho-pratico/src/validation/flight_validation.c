#include "validation/flight_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils/utils_validation.h"
#include "validation/airport_validation.h"
#include "validation/aircraft_validation.h"

//======== Syntactic validation: Flights

bool validate_flight_id_flight(const char *flight_id) {
    if (!flight_id) return false;

    int len = strlen(flight_id);

    if (len < 7 || len > 8) return false;

    for (int i = 0; i < 2; i++) {
        if (flight_id[i] < 'A' || flight_id[i] > 'Z') return false;
    }

    for (int i = 2; i < len; i++) {
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

bool validate_temporal_consistency(const char *status, long departure, long arrival, long actual_departure, long actual_arrival) {
    if (status && strcmp(status, "Cancelled") == 0) return true; 

    if (compare_datetimes(arrival, departure) < 0) return false;
    
    if (compare_datetimes(actual_arrival, actual_departure) < 0) return false;
    
    return true;
}

bool validate_cancelled_status(const char *status, const char *actual_departure_str, const char *actual_arrival_str) {
    if (!status || strcmp(status, "Cancelled") != 0) return true; 
    
    return (actual_departure_str && actual_arrival_str && strcmp(actual_departure_str, "N/A") == 0 && strcmp(actual_arrival_str, "N/A") == 0);
}


bool validate_delayed_status(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival) {
    if (!status || strcmp(status, "Delayed") != 0) return true; 

    if (compare_datetimes(actual_departure, scheduled_departure) < 0) return false;

    if (compare_datetimes(actual_arrival, scheduled_arrival) < 0) return false;
    
    return true;
}

/**
 * @brief Valida o status de um voo e a consistência dos seus horários.
 * 
 * Esta função realiza uma validação completa do status de um voo, verificando
 * se os dados associados ao status são consistentes. Os status aceites são
 * "On Time", "Delayed" e "Cancelled", cada um com suas regras específicas de
 * validação. Voos "On Time" são sempre válidos, voos "Delayed" devem ter
 * horários reais posteriores aos programados, e voos "Cancelled" devem ter
 * horários reais marcados como "N/A".
 * 
 * @param status String contendo o status do voo ("On Time", "Delayed" ou "Cancelled").
 * @param scheduled_departure Data e hora programada de partida no formato YYYYMMDDHHmm.
 * @param scheduled_arrival Data e hora programada de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure Data e hora real de partida no formato YYYYMMDDHHmm.
 * @param actual_arrival Data e hora real de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure_str String do horário real de partida (para validação de "N/A").
 * @param actual_arrival_str String do horário real de chegada (para validação de "N/A").
 * 
 * @return true se o status é válido e os dados são consistentes com o status.
 * @return false se o status é NULL, não é reconhecido, ou se os dados são
 *         inconsistentes com o status declarado.
 * 
 * @see validate_delayed_status
 * @see validate_cancelled_status
 */
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

bool validate_flight_logical(const char *origin, const char *destination, long departure, long arrival, long actual_departure, long actual_arrival,
                             const char *status, const char *aircraft, AircraftCatalog* aircraft_catalog, const char *actual_departure_str,
                             const char *actual_arrival_str) {
    
    if (!validate_destination_origin(origin, destination)) return false;
    
    if (!validate_temporal_consistency(status, departure, arrival, actual_departure, actual_arrival)) return false;
    
    if (!validate_aircraft_flight(aircraft, aircraft_catalog)) return false;
    
    if (!validate_status_flight_new(status, departure, arrival, actual_departure, actual_arrival, actual_departure_str, actual_arrival_str)) return false;
    
    return true;
}

bool is_canceled(char *status) {
    return (strcmp(status,"Cancelled") == 0);
}

bool is_delayed(char *status) {
    return (strcmp(status,"Delayed") == 0);
}