#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct airport_catalog {
    GHashTable* airports_by_code;
    GHashTable* flights_by_airport;
};

AirportCatalog* airport_catalog_create() {
    AirportCatalog* manager = malloc(sizeof(AirportCatalog));
    manager->airports_by_code = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_airport);
    manager->flights_by_airport = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

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


Airport* get_airport_by_code(AirportCatalog* manager, const char* code) {
    if (manager == NULL || code == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->airports_by_code, code); // retorna null se n existir
}

int airport_catalog_get_count(AirportCatalog* manager) {
    if (manager == NULL) {
        return 0;
    }

    return g_hash_table_size(manager->airports_by_code);
}


void airport_flights_counter_increment(const char* origin, int date, AirportCatalog* manager) {
    if (!manager || !origin || !*origin) return;
    
    // Criar chave composta "YYYYMMDD:AIRPORT_CODE"
    char key[20];
    snprintf(key, sizeof(key), "%d:%s", date, origin);
    
    gpointer stored_key = NULL;
    gpointer stored_val = NULL;

    if (g_hash_table_lookup_extended(manager->flights_by_airport, key, &stored_key, &stored_val)) {
        g_hash_table_steal(manager->flights_by_airport, stored_key);
        g_hash_table_insert(manager->flights_by_airport, stored_key, GINT_TO_POINTER(GPOINTER_TO_INT(stored_val) + 1));
    } else {
        g_hash_table_insert(manager->flights_by_airport, g_strdup(key), GINT_TO_POINTER(1));
    }
}

GHashTable* get_flights_by_origin(AirportCatalog* manager) {
    if (!manager) return NULL;
    return manager->flights_by_airport;
}

