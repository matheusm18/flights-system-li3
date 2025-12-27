#ifndef RESERVATION_CATALOG_H
#define RESERVATION_CATALOG_H

#include <glib.h>
#include "entities/reservation.h"

typedef struct reservation_stats_iterator ReservationStatsIter;
typedef struct reservation_catalog ReservationCatalog;

ReservationCatalog* reservation_catalog_create();
void reservation_catalog_destroy(ReservationCatalog* manager);
void reservation_catalog_add(ReservationCatalog* manager, Reservation* reservation);
void reservation_catalog_add_nationality_increment (ReservationCatalog* manager, const char* nationality, const char* airport_id);

ReservationStatsIter* reservation_catalog_create_stats_iter(ReservationCatalog* manager, const char* nationality);
int reservation_catalog_stats_iter_next(ReservationStatsIter* it, const char** airport_id, int* count);
void reservation_catalog_stats_iter_destroy(ReservationStatsIter* it);

Reservation* get_reservation_by_id(ReservationCatalog* manager, char* reservation_id);
#endif