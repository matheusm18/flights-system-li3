#include "entities/reservation.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// temos a estrutura mas já não armazenamos as reservas
struct reservation {
    char* reservation_id;
    char** flight_ids;
    char* document_number;
    char* seat;
    double price;
    bool extra_luggage;
    bool priority_boarding;
    char* qr_code;
};

Reservation* create_reservation(const char* reservation_id, char** flight_ids, const char* document_number, const char* seat, double price, bool extra_luggage, bool priority_boarding) {

    Reservation* reservation = malloc(sizeof(Reservation));
    if (reservation == NULL) return NULL;
    
    reservation->reservation_id = (char*) reservation_id;
    reservation->document_number = (char*) document_number;
    reservation->seat = (char*) seat;
    reservation->price = price;
    reservation->extra_luggage = extra_luggage;
    reservation->priority_boarding = priority_boarding;

    reservation->flight_ids = malloc(2 * sizeof(char*));

    for (int i = 0; i < 2; i++) {
        if (flight_ids && flight_ids[i] != NULL) {
            reservation->flight_ids[i] = strdup(flight_ids[i]);
        } else {
            reservation->flight_ids[i] = NULL; 
        }
    }

    return reservation;
}

void destroy_reservation(Reservation* r) {
    if (r == NULL) return;

    if (r->flight_ids) {
        for (int i = 0; i < 2; i++) {
            if (r->flight_ids[i] != NULL) {
                free(r->flight_ids[i]);
            }
        }
        free(r->flight_ids);
    }

    free(r);
}

char* get_reservation_identifier(const Reservation* r) { 
    if (r == NULL) return NULL;
    return strdup(r->reservation_id); 
}

