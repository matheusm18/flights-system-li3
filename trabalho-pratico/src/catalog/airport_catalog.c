#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct airport_data {
    Airport* airport;
    GPtrArray* departing_flights;
};

struct airport_catalog {
    GHashTable* airport_data_by_code;
};

void destroy_airport_data(gpointer data) {
    AirportData* ad = (AirportData*)data;
    if (ad != NULL) {
        destroy_airport(ad->airport);
        
        if (ad->departing_flights) g_ptr_array_unref(ad->departing_flights); // liberta o array mas sem libertaros voos lá dentro
        
        free(ad);
    }
}

AirportCatalog* airport_catalog_create() {
    AirportCatalog* manager = malloc(sizeof(AirportCatalog));
    manager->airport_data_by_code = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_airport_data);

    return manager;
}

void airport_catalog_destroy(AirportCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->airport_data_by_code);
        free(manager);
    }
}

void airport_catalog_add(AirportCatalog* manager, Airport* airport) {
    if (!manager || !airport) return;
    
    const char* code = get_airport_code(airport);
    if (code != NULL) {
        AirportData* data = malloc(sizeof(AirportData));
        data->airport = airport;
        data->departing_flights = g_ptr_array_new();

        g_hash_table_insert(manager->airport_data_by_code, g_strdup(code), data);
    }
}

void airport_catalog_add_flight(AirportCatalog* manager, const char* code, Flight* flight) {
    if (!manager || !code || !flight) return;

    AirportData* data = g_hash_table_lookup(manager->airport_data_by_code, code);
    
    if (data != NULL) g_ptr_array_add(data->departing_flights, flight);
}

int compare_flight_actual_departure(const void* a, const void* b) {
    Flight* f1 = *(Flight**)a;
    Flight* f2 = *(Flight**)b;

    long dt1 = get_flight_actual_departure(f1);
    long dt2 = get_flight_actual_departure(f2);

    if (dt1 <= 0 && dt2 <= 0) return 0;
    if (dt1 <= 0) return 1;   
    if (dt2 <= 0) return -1; 

    if (dt1 < dt2) return -1;
    if (dt1 > dt2) return 1;
    return 0;
}

void airport_catalog_sort_all_flights(AirportCatalog* catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, catalog->airport_data_by_code);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        AirportData* data = (AirportData*)value;

        if (data->departing_flights != NULL && data->departing_flights->len > 0) g_ptr_array_sort(data->departing_flights, compare_flight_actual_departure);
    }
}

void airport_catalog_iter_init(const AirportCatalog* catalog, GHashTableIter* iter) {
    if (!catalog || !iter) return;
    g_hash_table_iter_init(iter, catalog->airport_data_by_code);
}

const AirportData* airport_catalog_iter_next(GHashTableIter* iter) {
    gpointer key, value;

    if (g_hash_table_iter_next(iter, &key, &value)) {
        return (AirportData*)value;
    }
    return NULL;
}

void airport_catalog_departing_iter_init(guint* index) {
    *index = 0;
}

const Flight* airport_catalog_departing_iter_next(const AirportData* data, guint* index) {
    if (!data || !data->departing_flights || *index >= data->departing_flights->len) {
        return NULL;
    }
    return g_ptr_array_index(data->departing_flights, (*index)++);
}

const Airport* get_airport_by_code(AirportCatalog* manager, char* code) {
    if (manager == NULL || code == NULL) {
        return NULL;
    }

    AirportData* data = g_hash_table_lookup(manager->airport_data_by_code, code);
    if (data == NULL) return NULL;
    else return data->airport;
}

int airport_catalog_get_count(AirportCatalog* manager) {
    if (manager == NULL) return 0;
    return g_hash_table_size(manager->airport_data_by_code);
}

const Airport* get_airport_from_data(const AirportData* data) {
    return data ? data->airport : NULL;
}