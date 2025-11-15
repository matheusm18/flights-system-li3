#include "utils/reservation_validation.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils/flight_validation.h"

//======== Syntactic validation: Reservations

bool validate_reservation_id(const char *reservation_id) {
    if (!reservation_id || strlen(reservation_id) != 10) return false;

    if (reservation_id[0] != 'R') return false;

    for (int i = 1; i < 10; i++) {
        if (!isdigit(reservation_id[i])) return false;
    }
    return true;
}

bool validate_flight_ids_reservation(const char *flight_ids, FlightCatalog* manager) {
    if (!flight_ids || (strlen(flight_ids) != 11 && strlen(flight_ids) != 22)) return false;

    size_t len = strlen(flight_ids);

    int id_len = 8;

    char flight_id1 [id_len];

    if (len == 11) {
        if (flight_ids[0] != '[' || flight_ids[1] != '\'' || flight_ids[9] != '\'' || flight_ids[10] != ']') return false;
        
        for (int i = 0; i < id_len - 1; i ++) {
            flight_id1[i] = flight_ids[i+2];
        }

        flight_id1[id_len - 1] = '\0';

        if (get_flight_by_flight_id_from_catalog(manager, flight_id1) == NULL) return false;
    }

    char flight_id2 [id_len];

    if (len == 22) {
        if (flight_ids[0] != '[' || flight_ids[1] != '\'' || flight_ids[9] != '\'' || flight_ids[10] != ',' || flight_ids[11] != ' ' || flight_ids[12] != '\'' ||
            flight_ids[20] != '\'' || flight_ids[21] != ']') return false;

        for (int i = 0; i < id_len - 1; i ++) {
            flight_id1[i] = flight_ids[i+2];
        }

        flight_id1[id_len - 1] = '\0';

        for (int i = 0; i < id_len - 1; i ++) {
            flight_id2[i] = flight_ids[i+13];
        }

        flight_id2[id_len - 1] = '\0';

        if (get_flight_by_flight_id_from_catalog(manager, flight_id1) == NULL || get_flight_by_flight_id_from_catalog(manager, flight_id2) == NULL) return false;
        if (strcmp(flight_catalog_get_destination(manager, flight_id1), flight_catalog_get_origin(manager, flight_id2)) != 0) return false;
    }
    return true;
}

bool validate_document_number_reservation(const char *document_number, PassengerCatalog* manager) {
    return (passenger_catalog_dnumber_exists(manager, atoi(document_number)));
}