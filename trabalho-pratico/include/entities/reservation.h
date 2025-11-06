#ifndef RESERVATION_H
#define RESERVATION_H

typedef struct reservation Reservation;

Reservation* create_reservation(const char* reservation_id, char** flight_ids,const char* document_number, const char* seat, double price, int extra_luggage, int priority_boarding, int flight_count);

void destroy_reservation(Reservation* r);

#endif