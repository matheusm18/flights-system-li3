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
    manager->flights_by_airport = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)g_hash_table_destroy);

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


    GHashTable* dates = g_hash_table_lookup(manager->flights_by_airport, origin);
    if (!dates) {
        dates = g_hash_table_new(g_direct_hash, g_direct_equal);
        g_hash_table_insert(manager->flights_by_airport, g_strdup(origin), dates);
    }
    
    // Aumentar o counter para uma data em específico
    int current = GPOINTER_TO_INT(g_hash_table_lookup(dates, GINT_TO_POINTER(date)));
    g_hash_table_insert(dates, GINT_TO_POINTER(date), GINT_TO_POINTER(current + 1));
}

GHashTable* get_flights_by_origin(AirportCatalog* manager) {
    if (!manager) return NULL;
    return manager->flights_by_airport;
}

