#ifndef FLIGHT_CATALOG_H
#define FLIGHT_CATALOG_H

#include <glib.h>
#include "entities/flight.h"

typedef struct flight_catalog FlightCatalog;

FlightCatalog* flight_catalog_create(void);
void flight_catalog_destroy(FlightCatalog* manager);
void flight_catalog_add(FlightCatalog* manager, Flight* flight);
Flight* flight_catalog_get_by_flight_id(FlightCatalog* manager, const char* flight_id);
int flight_catalog_get_count(FlightCatalog* manager);

GHashTable* flight_catalog_get_all_flights(FlightCatalog* manager);

#endif
