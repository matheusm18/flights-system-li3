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
    manager->passenger_catalog = passenger_catalog_create();
    manager->reservation_catalog = reservation_catalog_create();
    return manager;
}

void catalog_manager_destroy(CatalogManager* manager) {
    airport_catalog_destroy(manager->airport_catalog);
    aircraft_catalog_destroy(manager->aircraft_catalog);
    flight_catalog_destroy(manager->flight_catalog);
    passenger_catalog_destroy(manager->passenger_catalog);
    reservation_catalog_destroy(manager->reservation_catalog);
    free(manager);
}

AirportCatalog* get_airports_from_catalog_manager(const CatalogManager* manager) {
    return manager->airport_catalog;
}

AircraftCatalog* get_aircrafts_from_catalog_manager(const CatalogManager* manager) {
    return manager->aircraft_catalog;
}

FlightCatalog* get_flights_from_catalog_manager(const CatalogManager* manager) {
    return manager->flight_catalog;
}

PassengerCatalog* get_passengers_from_catalog_manager(const CatalogManager* manager) {
    return manager->passenger_catalog;
}

ReservationCatalog* get_reservations_from_catalog_manager(const CatalogManager* manager) {
    return manager->reservation_catalog;
}