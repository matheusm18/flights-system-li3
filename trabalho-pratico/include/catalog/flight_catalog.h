#ifndef FLIGHT_CATALOG_H
#define FLIGHT_CATALOG_H

#include <glib.h>
#include "entities/flight.h"

typedef struct flight_catalog FlightCatalog;

FlightCatalog* flight_catalog_create(void);
void flight_catalog_destroy(FlightCatalog* manager);
void flight_catalog_add(FlightCatalog* manager, Flight* flight);
Flight* get_flight_by_flight_id_from_catalog(FlightCatalog* manager, const char* flight_id);
const char* flight_catalog_get_origin(const FlightCatalog* manager, const char* flight_id);
const char* flight_catalog_get_destination(const FlightCatalog* manager, const char* flight_id);
int flight_catalog_get_count(FlightCatalog* manager);

GHashTable* get_flight_catalog(FlightCatalog* manager);

#endif
