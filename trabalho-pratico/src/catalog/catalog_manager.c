#include "catalog/catalog_manager.h"
#include <stdlib.h>
#include <string.h>

struct CatalogManager {
    AirportCatalog* airport_catalog;
    AircraftCatalog* aircraft_catalog;
    FlightCatalog* flight_catalog;
    PassengerCatalog* passenger_catalog;
    ReservationCatalog* reservation_catalog;
};

CatalogManager* catalog_manager_create() {
    CatalogManager* manager = malloc(sizeof(CatalogManager));
    manager->airport_catalog = airport_catalog_create();
    manager->aircraft_catalog = aircraft_catalog_create();
    manager->flight_catalog = flight_catalog_create();
    //manager->passenger_catalog = passenger_catalog_create();
    //manager->reservation_catalog = reservation_catalog_create();
    return manager;
}

void catalog_manager_destroy(CatalogManager* manager) {
    airport_catalog_destroy(manager->airport_catalog);
    aircraft_catalog_destroy(manager->aircraft_catalog);
    flight_catalog_destroy(manager->flight_catalog);
    //passenger_catalog_destroy(manager->passenger_catalog);
    //reservation_catalog_destroy(manager->reservation_catalog);
    free(manager);
}

AirportCatalog* catalog_manager_get_airports(const CatalogManager* manager) {
    return manager->airport_catalog;
}

AircraftCatalog* catalog_manager_get_aircrafts(const CatalogManager* manager) {
    return manager->aircraft_catalog;
}

FlightCatalog* catalog_manager_get_flights(const CatalogManager* manager) {
    return manager->flight_catalog;
}

PassengerCatalog* catalog_manager_get_passengers(const CatalogManager* manager) {
    return manager->passenger_catalog;
}

ReservationCatalog* catalog_manager_get_reservations(const CatalogManager* manager) {
    return manager->reservation_catalog;
}