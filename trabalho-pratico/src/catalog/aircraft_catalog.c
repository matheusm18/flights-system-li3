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
    
    gpointer stored_key = NULL;
    gpointer stored_val = NULL;

     // Usa lookup_extended para reutilizar a chave existente
    if (g_hash_table_lookup_extended(manager->flight_counts, aircraft_id, &stored_key, &stored_val)) {
        int new_count = GPOINTER_TO_INT(stored_val) + 1;
        // Steal remove sem libertar a chave, depois reinsere a MESMA chave
        g_hash_table_steal(manager->flight_counts, stored_key);
        g_hash_table_insert(manager->flight_counts, stored_key, GINT_TO_POINTER(new_count));
    } else {
        // Primeira vez - cria nova chave
        g_hash_table_insert(manager->flight_counts, g_strdup(aircraft_id), GINT_TO_POINTER(1));
    }
}

GHashTable* get_aircraft_flights_counter(AircraftCatalog* manager) {
    if (!manager) return NULL;
    return manager->flight_counts;
}



