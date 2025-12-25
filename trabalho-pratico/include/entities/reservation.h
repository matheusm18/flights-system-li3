#ifndef RESERVATION_H
#define RESERVATION_H

#include <stdbool.h>

typedef struct reservation Reservation;

Reservation* create_reservation(const char* reservation_id, char** flight_ids, const char* document_number, const char* seat, double price, bool extra_luggage, bool priority_boarding);

void destroy_reservation(Reservation* r);

char* get_reservation_identifier(const Reservation* r);

#endif