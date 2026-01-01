#include "catalog/catalog_manager.h"
#include <stdlib.h>
#include <string.h>

struct CatalogManager {
    AirportCatalog* airport_catalog;
    AircraftCatalog* aircraft_catalog;
    FlightCatalog* flight_catalog;
    PassengerCatalog* passenger_catalog;
    ReservationCatalog* reservation_catalog;
    GHashTable* string_pool;
};

CatalogManager* catalog_manager_create() {
    CatalogManager* manager = malloc(sizeof(CatalogManager));
    manager->airport_catalog = airport_catalog_create();
    manager->aircraft_catalog = aircraft_catalog_create();
    manager->flight_catalog = flight_catalog_create();
    manager->passenger_catalog = passenger_catalog_create();
    manager->reservation_catalog = reservation_catalog_create();

    /*
    A chave é a string. O valor é a mesma string (para economizar).
    'free' é a função que será chamada em cada string quando o manager for destruído.
    */
    manager->string_pool = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return manager;
}

void catalog_manager_destroy(CatalogManager* manager) {
    airport_catalog_destroy(manager->airport_catalog);
    aircraft_catalog_destroy(manager->aircraft_catalog);
    flight_catalog_destroy(manager->flight_catalog);
    passenger_catalog_destroy(manager->passenger_catalog);
    reservation_catalog_destroy(manager->reservation_catalog);

    g_hash_table_destroy(manager->string_pool);
    free(manager);
}

// getters

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

char* catalog_manager_get_string_from_pool(CatalogManager* manager, const char* str) {
    if (!manager || !str) return NULL;

    // verifica se a string já existe na pool
    char* stored = g_hash_table_lookup(manager->string_pool, str);
    
    if (stored != NULL) {
        return stored; // retorna o ponteiro já existente
    }

    // se não existe, duplica, insere e retorna
    char* new_str = strdup(str);
    
    g_hash_table_insert(manager->string_pool, new_str, new_str);
    
    return new_str;
}