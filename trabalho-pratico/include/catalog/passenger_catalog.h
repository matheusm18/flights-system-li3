#ifndef PASSENGER_CATALOG_H
#define PASSENGER_CATALOG_H

#include <glib.h>
#include "entities/passenger.h"

typedef struct passenger_catalog PassengerCatalog;

PassengerCatalog* passenger_catalog_create();
void passenger_catalog_destroy(PassengerCatalog* manager);
void passenger_catalog_add(PassengerCatalog* manager, Passenger* passenger);
Passenger* get_passenger_by_dnumber(PassengerCatalog* manager, const char* dnumber);

#endif