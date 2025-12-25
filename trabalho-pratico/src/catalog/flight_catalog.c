#include "catalog/flight_catalog.h"
#include "entities/flight.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct airline_stats {
    char* airline_id;
    long total_delay;
    int delayed_flights_count;
    double media;
};

struct flight_catalog {
    GHashTable* flights_by_flight_id;
    GHashTable* airline_lookup;
    GPtrArray* airline_array;
};

FlightCatalog* flight_catalog_create() {
    FlightCatalog* manager = malloc(sizeof(FlightCatalog));
    manager->flights_by_flight_id = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_flight);
    manager->airline_lookup = g_hash_table_new(g_str_hash, g_str_equal);
    manager->airline_array = g_ptr_array_new_with_free_func((GDestroyNotify) destroy_airline);

    return manager;
}

void flight_catalog_destroy(FlightCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->flights_by_flight_id);
        g_hash_table_destroy(manager->airline_lookup);
        g_ptr_array_unref(manager->airline_array);
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

void flight_catalog_add_airline_stats(FlightCatalog* manager, char* airline_id) {
    if (!manager || !airline_id) return;

    AirlineStats* stats = malloc(sizeof(AirlineStats));
    stats->airline_id = strdup(airline_id);
    stats->total_delay = 0;
    stats->delayed_flights_count = 0;
    stats->media = 0;
    
    g_hash_table_insert(manager->airline_lookup, stats->airline_id, stats);
    g_ptr_array_add(manager->airline_array, stats);
}

void destroy_airline(AirlineStats* a) {
    if (!a) return;

    free(a->airline_id);
    free(a);
}

int compare_airlines(const void* a, const void* b) {
    AirlineStats *as = *(AirlineStats**)a;
    AirlineStats *bs = *(AirlineStats**)b;

    // calculo das médias (atraso total / número de voos)
    double mediaA = get_airline_stats_media(as);
    double mediaB = get_airline_stats_media(bs);

    // primeiro critério
    if (mediaA > mediaB) return -1;
    if (mediaA < mediaB) return 1;

    // segundo critério

    int result = strcmp(as->airline_id, bs->airline_id);
    return result;
}

void airline_stats_sort_array(FlightCatalog* manager) {
    if (manager == NULL) return;

    GPtrArray* array = manager->airline_array;
    if (!array || array->len == 0) return;

    g_ptr_array_sort(array, compare_airlines);
}

void airline_stats_increment(FlightCatalog* manager, char* airline_id, int delay) {
    if (!manager) return;

    AirlineStats* stats = get_airline_stats_by_identifier(manager, airline_id);
    if (stats != NULL) {
        stats->total_delay += delay;
        stats->delayed_flights_count++;
    }
}

void set_airline_stats_media(AirlineStats* s, double media) {
    if (s == NULL) return;

    s->media = media;
}

// getters

Flight* get_flight_by_flight_id_from_catalog(FlightCatalog* manager, const char* flight_id) {
    if (manager == NULL || flight_id == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->flights_by_flight_id, flight_id); 
}

char* flight_catalog_get_origin(const FlightCatalog* manager, const char* flight_id) {
    if (manager == NULL || flight_id == NULL) return NULL;

    Flight* f = g_hash_table_lookup(manager->flights_by_flight_id, flight_id);

    if (!f) return NULL;

    return get_flight_origin(f);
}

char* flight_catalog_get_destination(const FlightCatalog* manager, const char* flight_id) {
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

AirlineStats* get_airline_stats_by_identifier(FlightCatalog* manager, char* airline_id) {
    if (manager == NULL || airline_id == NULL) return NULL;

    AirlineStats* stats = g_hash_table_lookup(manager->airline_lookup, airline_id);
    if (stats == NULL) return NULL;
    else return stats;
}

AirlineStats* get_airline_stats_from_array(FlightCatalog* manager, int index) {
    if (manager == NULL) return NULL;

    GPtrArray* array = manager->airline_array;

    AirlineStats* stats = g_ptr_array_index(array, index);
    if (stats == NULL) return NULL;
    else return stats;
}

int get_airline_stats_array_length(FlightCatalog* manager) {
    if (manager == NULL) return 0;
    else return manager->airline_array->len;
}

char* get_airline_stats_airline(AirlineStats* a) {
    if (a == NULL) return NULL;
    else return strdup(a->airline_id);
}

int get_airline_stats_flights_count(AirlineStats* a) {
    if (a == NULL) return 0;
    else return a->delayed_flights_count;
}

long get_airline_stats_total_delay(AirlineStats* a) {
    if (a == NULL) return 0;
    else return a->total_delay;
}

double get_airline_stats_media(AirlineStats* a) {
    if (a == NULL) return 0;
    else return a->media;
}