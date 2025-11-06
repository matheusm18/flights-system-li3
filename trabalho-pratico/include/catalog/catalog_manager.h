#ifndef CATALOG_MANAGER_H
#define CATALOG_MANAGER_H

#include <glib.h>
#include "catalog/airport_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "catalog/flight_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/reservation_catalog.h"

typedef struct CatalogManager CatalogManager;

CatalogManager* catalog_manager_create(void);
void catalog_manager_destroy(CatalogManager* manager);

AirportCatalog* catalog_manager_get_airports(const CatalogManager* manager);
AircraftCatalog* catalog_manager_get_aircrafts(const CatalogManager* manager);
FlightCatalog* catalog_manager_get_flights(const CatalogManager* manager);
PassengerCatalog* catalog_manager_get_passengers(const CatalogManager* manager);
ReservationCatalog* catalog_manager_get_reservations(const CatalogManager* manager);

#endif