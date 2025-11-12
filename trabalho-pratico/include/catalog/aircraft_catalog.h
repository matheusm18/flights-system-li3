#ifndef AIRCRAFT_CATALOG_H
#define AIRCRAFT_CATALOG_H

#include <glib.h>
#include "entities/aircraft.h"

typedef struct aircraft_catalog AircraftCatalog;

AircraftCatalog* aircraft_catalog_create();
void aircraft_catalog_destroy(AircraftCatalog* manager);
void aircraft_catalog_add(AircraftCatalog* manager, Aircraft* aircraft);
Aircraft* get_aircraft_by_identifier(AircraftCatalog* manager, const char* identifier);
void aircrafts_counter_increment(const char* aircraft_id, AircraftCatalog* manager);
int get_total_aircrafts_in_catalog(AircraftCatalog* catalog);
GHashTable* aircraft_catalog_get_aircrafts(AircraftCatalog* catalog);

#endif