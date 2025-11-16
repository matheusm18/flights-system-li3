#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct airport_catalog {
    GHashTable* airports_by_code;
};

AirportCatalog* airport_catalog_create() {
    AirportCatalog* manager = malloc(sizeof(AirportCatalog));
    manager->airports_by_code = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_airport);

    return manager;
}

void airport_catalog_destroy(AirportCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->airports_by_code);
        free(manager);
    }
}

void airport_catalog_add(AirportCatalog* manager, Airport* airport) {
    if (!manager || !airport) return;
    
    const char* code = get_airport_code(airport);
    if (code != NULL) {
        g_hash_table_insert(manager->airports_by_code, g_strdup(code), airport);
    }
}

void airport_catalog_sort_all_flights(AirportCatalog* catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, catalog->airports_by_code);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Airport* airport = (Airport*) value;
        airport_sort_departing_flights(airport);
    }
}

void airport_catalog_iter_init(const AirportCatalog* catalog, GHashTableIter* iter) {
    if (!catalog || !iter) return;
    g_hash_table_iter_init(iter, catalog->airports_by_code);
}

const Airport* airport_catalog_iter_next(GHashTableIter* iter) {
    gpointer key, value;
    if (g_hash_table_iter_next(iter, &key, &value)) return (Airport*) value;
    return NULL;
}

const Airport* get_airport_by_code(AirportCatalog* manager, const char* code) {
    if (manager == NULL || code == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->airports_by_code, code); 
}

int airport_catalog_get_count(AirportCatalog* manager) {
    if (manager == NULL) {
        return 0;
    }

    return g_hash_table_size(manager->airports_by_code);
}
