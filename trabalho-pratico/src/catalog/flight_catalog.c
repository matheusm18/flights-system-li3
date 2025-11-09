#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include <stdlib.h>
#include <string.h>

struct flight_catalog {
    GHashTable* flights_by_flight_id;
    GHashTable* flights_by_origin;
};

FlightCatalog* flight_catalog_create() {
    FlightCatalog* manager = malloc(sizeof(FlightCatalog));
    manager->flights_by_flight_id = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_flight);

    return manager;
}

void flight_catalog_destroy(FlightCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->flights_by_flight_id);
        free(manager);
    }
}

void flight_catalog_add(FlightCatalog* manager, Flight* flight) {
    if (!manager || !flight) return;
    
    const char* flight_id = get_flight_id(flight);
    if (flight_id != NULL) {
        g_hash_table_insert(manager->flights_by_flight_id, g_strdup(flight_id), flight);
    }
}

Flight* get_flight_by_flight_id_from_catalog(FlightCatalog* manager, const char* flight_id) {
    if (manager == NULL || flight_id == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->flights_by_flight_id, flight_id); // retorna null se n existir
}

const char* flight_catalog_get_origin(const FlightCatalog* manager, const char* flight_id) {
    if (manager == NULL || flight_id == NULL) return NULL;

    Flight* f = g_hash_table_lookup(manager->flights_by_flight_id, flight_id);

    if (!f) return NULL;

    return get_flight_origin(f);
}

const char* flight_catalog_get_destination(const FlightCatalog* manager, const char* flight_id) {
    if (manager == NULL || flight_id == NULL) return NULL;

    Flight* f = g_hash_table_lookup(manager->flights_by_flight_id, flight_id);
    
    if (!f) return NULL;

    return get_flight_destination(f);
}


int flight_catalog_get_count(FlightCatalog* manager) {
    if (manager == NULL) {
        return 0;
    }

    return g_hash_table_size(manager->flights_by_flight_id);
}

GHashTable* get_flight_catalog(FlightCatalog* manager) {
    if (!manager) return NULL;
    return manager->flights_by_flight_id;
}



