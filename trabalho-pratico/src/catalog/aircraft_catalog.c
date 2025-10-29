#include "catalog/aircraft_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/aircraft.h"

struct aircraft_catalog {
    GHashTable* aircraft_by_identifier;
};

AircraftCatalog* aircraft_catalog_create() {
    AircraftCatalog* manager = malloc(sizeof(AircraftCatalog));
    manager->aircraft_by_identifier = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_aircraft);

    return manager;
}

void aircraft_catalog_destroy(AircraftCatalog* manager){
    if (manager != NULL) {
        g_hash_table_destroy(manager->aircraft_by_identifier);
        free(manager);
    }
}

void aircarft_catalog_add(AircraftCatalog* manager, Aircraft* aircraft) {
    if (!manager || !aircraft) return;
    
    const char* identifier = aircraft_get_identifier(aircraft);
    if (identifier != NULL) {
        g_hash_table_insert(manager->aircraft_by_identifier, g_strdup(identifier), aircraft);
    }
}

Aircraft* get_aircraft_by_identifier(AircraftCatalog* manager, const char* identifier) {
    if (manager == NULL || identifier == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->aircraft_by_identifier, identifier); 
}  






