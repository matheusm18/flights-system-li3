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
    if (!flight_ids) return false;

    int len = strlen(flight_ids);
    if (len < 2 || flight_ids[0] != '[' || flight_ids[len - 1] != ']') return false;

    char *ids = strdup(flight_ids);
    char *delims = "[]' ,\""; 
    char *id1 = strtok(ids, delims);
    char *id2 = strtok(NULL, delims);
    char *id3 = strtok(NULL, delims); // para garantir que não há um 3 voo

    if (id1 == NULL || id3 != NULL) {
        free(ids); 
        return false; 
    }

    if (!validate_flight_id_flight(id1)) {
        free(ids); 
        return false;
    }

    Flight* f1 = get_flight_by_flight_id_from_catalog(manager, id1);
    if (f1 == NULL) { 
        free(ids); 
        return false; 
    }

    if (id2 != NULL) {

        if (!validate_flight_id_flight(id2)) {
            free(ids); 
            return false;
        }

        Flight* f2 = get_flight_by_flight_id_from_catalog(manager, id2);

        if (f2 == NULL) { 
            free(ids); 
            return false; 
        }

        char *dest1 = get_flight_destination(f1);
        char *orig2 = get_flight_origin(f2);
        
        if (strcmp(dest1, orig2) != 0) {
            free(dest1); 
            free(orig2);
            free(ids);
            return false; 
        }

        free(dest1); 
        free(orig2);
    }

    free(ids);
    return true;
}

bool validate_document_number_reservation(const char *document_number, PassengerCatalog* manager) {
    return (passenger_catalog_dnumber_exists(manager, atoi(document_number)));
}