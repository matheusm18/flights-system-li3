#ifndef AIRPORT_CATALOG_H
#define AIRPORT_CATALOG_H

#include <glib.h>
#include "entities/airport.h"

typedef struct airport_catalog AirportCatalog;

AirportCatalog* airport_catalog_create(void);
void airport_catalog_destroy(AirportCatalog* manager);
void airport_catalog_add(AirportCatalog* manager, Airport* airport);
void airport_catalog_sort_all_flights(AirportCatalog* catalog);
void airport_catalog_iter_init(const AirportCatalog* catalog, GHashTableIter* iter);
const Airport* airport_catalog_iter_next(GHashTableIter* iter);

// getters
const Airport* get_airport_by_code(AirportCatalog* manager, const char* code);
int airport_catalog_get_count(AirportCatalog* manager);

#endif
