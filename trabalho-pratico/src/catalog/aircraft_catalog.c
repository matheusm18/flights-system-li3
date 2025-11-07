#include "catalog/aircraft_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/aircraft.h"

struct aircraft_catalog {
    GHashTable* aircraft_by_identifier;
        GHashTable* flight_counts;
};

AircraftCatalog* aircraft_catalog_create() {
    AircraftCatalog* manager = malloc(sizeof(AircraftCatalog));
    manager->aircraft_by_identifier = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_aircraft);
    manager->flight_counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    return manager;
}

void aircraft_catalog_destroy(AircraftCatalog* manager){
    if (manager != NULL) {
        g_hash_table_destroy(manager->aircraft_by_identifier);
        g_hash_table_destroy(manager->flight_counts);
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

Aircraft* get_aircraft_by_identifier(AircraftCatalog* manager, const char* identifier) {
    if (manager == NULL || identifier == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->aircraft_by_identifier, identifier); 
} 

void aircrafts_counter_increment(const char* aircraft_id, AircraftCatalog* manager){
    if (!manager || !aircraft_id || !*aircraft_id) return;
    
    if (!g_hash_table_lookup(manager->aircraft_by_identifier, aircraft_id)) {
        return;
    }
    
    gpointer current_count = g_hash_table_lookup(manager->flight_counts, aircraft_id);

    if (current_count) {
        g_hash_table_replace(manager->flight_counts, g_strdup(aircraft_id), GINT_TO_POINTER(GPOINTER_TO_INT(current_count) + 1));
    } else {
        g_hash_table_insert(manager->flight_counts, g_strdup(aircraft_id), GINT_TO_POINTER(GPOINTER_TO_INT(1)));
    }
}

GHashTable* get_aircraft_flights_counter(AircraftCatalog* manager) {
    if (!manager) return NULL;
    return manager->flight_counts;
}



