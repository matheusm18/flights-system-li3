#ifndef PASSENGER_CATALOG_H
#define PASSENGER_CATALOG_H

#include <glib.h>
#include "entities/passenger.h"

typedef struct passenger_catalog PassengerCatalog;

PassengerCatalog* passenger_catalog_create();
void passenger_catalog_destroy(PassengerCatalog* manager);
void passenger_catalog_add(PassengerCatalog* manager, int document_number);
int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number);

#endif