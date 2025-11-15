#include "entities/reservation.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// não está em comentario porque nem se quer criamos reservas para já
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

Reservation* create_reservation(const char* reservation_id, char** flight_ids,const char* document_number, const char* seat, double price, int extra_luggage, int priority_boarding) {

    Reservation* reservation = malloc(sizeof(Reservation));
    if (reservation == NULL) return NULL;
    
    reservation->reservation_id = strdup(reservation_id);
    reservation->document_number = strdup(document_number);
    reservation->seat = strdup(seat);
    reservation->price = price;
    reservation->extra_luggage = extra_luggage;
    reservation->priority_boarding = priority_boarding;

    reservation->flight_ids = malloc(2 * sizeof(char*));

    for (int i = 0; i < 2; i++) {
        if (flight_ids[i] != NULL) {
            reservation->flight_ids[i] = strdup(flight_ids[i]);
        } else {
            reservation->flight_ids[i] = NULL; // NULL para indicar que não há mais voos
        }
    }

    return reservation;
}

void destroy_reservation(Reservation* r) {
    if (r == NULL) return;

    free(r->reservation_id);
    free(r->document_number);
    free(r->seat);

    if (r->flight_ids) {
        for (int i = 0; i < 2 && r->flight_ids[i] != NULL; i++) {
            free(r->flight_ids[i]);
        }
        free(r->flight_ids);
    }

    free(r);
}