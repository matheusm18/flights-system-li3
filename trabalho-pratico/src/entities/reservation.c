#include "entities/reservation.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct reservation {
    char* reservation_id;
    char** flight_ids;
    char* document_number;
    char* seat;
    double price;
    bool extra_luggage;
    bool priority_boarding;
    // qr_code não é armazenado para já
};

// passar o contador para poder alocar a memória? não estou a ver outra maneira
Reservation* create_reservation(const char* reservation_id, char** flight_ids,const char* document_number, const char* seat, double price, int extra_luggage, int priority_boarding, int flight_count) {

    Reservation* reservation = malloc(sizeof(Reservation));
    if (reservation == NULL) return NULL;
    
    reservation->reservation_id = strdup(reservation_id);
    reservation->document_number = strdup(document_number);
    reservation->seat = strdup(seat);
    reservation->price = price;
    reservation->extra_luggage = extra_luggage;
    reservation->priority_boarding = priority_boarding;

    reservation->flight_ids = malloc(flight_count * sizeof(char*));

    for (int i = 0; i < flight_count; i++) {
        reservation->flight_ids[i] = strdup(flight_ids[i]);
    }

    return reservation;
}

void destroy_reservation(Reservation* r) {
    if (r != NULL) {

        free(r->reservation_id);

        if (r->flight_ids) {
            for (int i = 0; r->flight_ids[i] != NULL; i++) free(r->flight_ids[i]);
            free(r->flight_ids);
        }

        free(r->document_number);
        free(r->seat);

        free(r);
    }
}
