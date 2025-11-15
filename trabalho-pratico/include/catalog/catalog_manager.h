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

// getters
AirportCatalog* get_airports_from_catalog_manager(const CatalogManager* manager);
AircraftCatalog* get_aircrafts_from_catalog_manager(const CatalogManager* manager);
FlightCatalog* get_flights_from_catalog_manager(const CatalogManager* manager);
PassengerCatalog* get_passengers_from_catalog_manager(const CatalogManager* manager);
ReservationCatalog* get_reservations_from_catalog_manager(const CatalogManager* manager);

#endif