#ifndef RESERVATION_CATALOG_H
#define RESERVATION_CATALOG_H

#include <glib.h>
#include "entities/reservation.h"

typedef struct reservation_catalog ReservationCatalog;

ReservationCatalog* reservation_catalog_create();
void reservation_catalog_destroy(ReservationCatalog* manager);
void reservation_catalog_add(ReservationCatalog* manager, Reservation* reservation);
Reservation* get_reservation_by_id(ReservationCatalog* manager, char* reservation_id);

#endif