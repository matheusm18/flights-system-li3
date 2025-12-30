#include "catalog/aircraft_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/aircraft.h"

struct aircraft_data {
    Aircraft* aircraft;
    int flight_count;
};

struct aircraft_iterator {
    GHashTableIter iter;
};

struct aircraft_catalog {
    GHashTable* aircraft_data_by_identifier;
};

void destroy_aircraft_data(gpointer data) {
    AircraftData* ad = (AircraftData*)data;
    if (ad != NULL) {
        destroy_aircraft(ad->aircraft);
        free(ad);
    }
}

AircraftCatalog* aircraft_catalog_create() {
    AircraftCatalog* manager = malloc(sizeof(AircraftCatalog));
    manager->aircraft_data_by_identifier = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_aircraft_data);

    return manager;
}

void aircraft_catalog_destroy(AircraftCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->aircraft_data_by_identifier);
        free(manager);
    }
}

void aircraft_catalog_add(AircraftCatalog* manager, Aircraft* aircraft) {
    if (!manager || !aircraft) return;
    
    char* identifier = get_aircraft_identifier(aircraft);
    if (identifier != NULL) {
        AircraftData* data = malloc(sizeof(AircraftData));
        data->aircraft = aircraft;
        data->flight_count = 0;

        g_hash_table_insert(manager->aircraft_data_by_identifier, g_strdup(identifier), data);
        free(identifier);
    }
}


// parte dos iters

AircraftIter* aircraft_catalog_iter_create(const AircraftCatalog* catalog) {
    if (!catalog) return NULL;
    
    AircraftIter* it = malloc(sizeof(AircraftIter));
    g_hash_table_iter_init(&(it->iter), catalog->aircraft_data_by_identifier);
    return it;
}

const AircraftData* aircraft_catalog_iter_next(AircraftIter* it) {
    gpointer key, value;

    if (it && g_hash_table_iter_next(&(it->iter), &key, &value)) {
        return (const AircraftData*)value;
    }
    return NULL;
}

void aircraft_catalog_iter_free(AircraftIter* it) {
    if (it) free(it);
}



void aircrafts_counter_increment(const char* aircraft_id, AircraftCatalog* manager) {
    if (!manager || !aircraft_id || !*aircraft_id) return;

    AircraftData* data = g_hash_table_lookup(manager->aircraft_data_by_identifier, aircraft_id);
    if (data != NULL) {
        data->flight_count++;
    }
}


// getters

const Aircraft* get_aircraft_by_identifier(AircraftCatalog* manager, const char* identifier) {
    if (manager == NULL || identifier == NULL) return NULL;

    AircraftData* data = g_hash_table_lookup(manager->aircraft_data_by_identifier,identifier);
    if (data == NULL) return NULL;
    else return data->aircraft;
} 

int get_aircraft_flight_count(const AircraftData* data) {
    return data ? data->flight_count : 0;
}

int get_total_aircrafts_in_catalog(AircraftCatalog* catalog) {
    if (!catalog) return 0;
    return g_hash_table_size(catalog->aircraft_data_by_identifier);
}

const Aircraft* get_aircraft_from_data(const AircraftData* data) {
    return data ? data->aircraft : NULL;
}