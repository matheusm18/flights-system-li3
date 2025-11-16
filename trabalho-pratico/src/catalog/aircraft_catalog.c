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

void aircraft_catalog_add(AircraftCatalog* manager, Aircraft* aircraft) {
    if (!manager || !aircraft) return;
    
    const char* identifier = get_aircraft_identifier(aircraft);
    if (identifier != NULL) {
        g_hash_table_insert(manager->aircraft_by_identifier, g_strdup(identifier), aircraft);
    }
}

void aircraft_catalog_iter_init(const AircraftCatalog* catalog, GHashTableIter* iter) {
    if (!catalog || !iter) return;
    g_hash_table_iter_init(iter, catalog->aircraft_by_identifier);
}

const Aircraft* aircraft_catalog_iter_next(GHashTableIter* iter) {
    gpointer key, value;
    if (!iter) return NULL;

    if (g_hash_table_iter_next(iter, &key, &value)) {
        return (const Aircraft*) value;
    }
    return NULL;  // acabou
}

Aircraft* get_aircraft_by_identifier(AircraftCatalog* manager, const char* identifier) {
    if (manager == NULL || identifier == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->aircraft_by_identifier, identifier); 
} 

void aircrafts_counter_increment(const char* aircraft_id, AircraftCatalog* manager) {
    if (!manager || !aircraft_id || !*aircraft_id) return;

    Aircraft* aircraft = get_aircraft_by_identifier(manager, aircraft_id);
    if (aircraft != NULL) {
        // incrementa o contador diretamente na entidade
        aircraft_increment_flight_count(aircraft);
    }
}

int get_total_aircrafts_in_catalog(AircraftCatalog* catalog) {
    if (!catalog) return 0;
    return g_hash_table_size(catalog->aircraft_by_identifier);
}
