#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// struct do iterador de aeroportos
struct airport_iterator {
    GHashTableIter iter;
};

// struct do iterador de flights (cada aeroporto tem a lista de voos que sairam de lá)
struct airport_flight_iterator {
    const AirportData* data;
    guint current_index;
};

struct airport_data {
    Airport* airport;
    int arrival_count;
    int departure_count;
    GPtrArray* departing_flights;
};

struct airport_catalog {
    GHashTable* airport_data_by_code;
};

static void destroy_airport_data(gpointer data) {
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
    
    char* code = get_airport_code(airport);
    if (code != NULL) {
        AirportData* data = malloc(sizeof(AirportData));
        data->airport = airport;
        data->arrival_count = 0;
        data->departure_count = 0;
        data->departing_flights = g_ptr_array_new();

        g_hash_table_insert(manager->airport_data_by_code, g_strdup(code), data);
        free(code);
    }
}

void airport_catalog_add_flight(AirportCatalog* manager, const char* code, Flight* flight) {
    if (!manager || !code || !flight) return;

    AirportData* data = g_hash_table_lookup(manager->airport_data_by_code, code);
    
    if (data != NULL) g_ptr_array_add(data->departing_flights, flight);
}

static int compare_flight_actual_departure(const void* a, const void* b) {
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

    GHashTableIter iter; // aqui não usamos o airportIter porque vamos precisar ordenar os dados (airportIter serve mais para as queries)
    gpointer key, value;
    g_hash_table_iter_init(&iter, catalog->airport_data_by_code);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        AirportData* data = (AirportData*)value; 

        if (data->departing_flights && data->departing_flights->len > 1) {
            g_ptr_array_sort(data->departing_flights, compare_flight_actual_departure);
        }
    }
}

void airport_passenger_increment(AirportCatalog* manager, char* airport_code, char type) {
    if (!manager || !airport_code || !*airport_code) return;

    AirportData* data = g_hash_table_lookup(manager->airport_data_by_code, airport_code);
    if (data != NULL) {
        if (type == 'a') data->arrival_count++;
        if (type == 'd') data->departure_count++;
    }
}

// funções para iterar aeroportos

AirportIter* airport_catalog_iter_create(const AirportCatalog* catalog) {
    if (!catalog) return NULL;

    AirportIter* it = malloc(sizeof(AirportIter));
    g_hash_table_iter_init(&(it->iter), catalog->airport_data_by_code);
    return it;
}

const AirportData* airport_catalog_iter_next(AirportIter* it) {
    gpointer key, value;
    if (it && g_hash_table_iter_next(&(it->iter), &key, &value)) {
        return (const AirportData*)value;
    }
    return NULL;
}

void airport_catalog_iter_free(AirportIter* it) {
    if (it) free(it);
}


// funções para iterar voos de um aeroporto (query3)


AirportFlightIter* airport_flight_iter_create(const AirportData* data) {
    if (!data) return NULL;

    AirportFlightIter* it = malloc(sizeof(struct airport_flight_iterator));
    it->data = data;
    it->current_index = 0;
    return it;
}

const Flight* airport_flight_iter_next(AirportFlightIter* it) {
    if (!it || !it->data->departing_flights || it->current_index >= it->data->departing_flights->len) {
        return NULL;
    }
    return g_ptr_array_index(it->data->departing_flights, it->current_index++);
}

void airport_flight_iter_free(AirportFlightIter* it) {
    if (it) free(it);
}

// binary search para encontrar primeiro voo >= start_date
static int find_first_flight_in_range(GPtrArray* flights, int start_date) {
    if (!flights || flights->len == 0) return -1;
    
    int i = 0;
    int j = flights->len - 1;
    int first_idx = -1;
    
    while (i <= j) {
        int mid = i + (j - i) / 2;
        Flight* flight = g_ptr_array_index(flights, mid);
        int flight_date = get_date_part(get_flight_actual_departure(flight));
        
        if (flight_date >= start_date) {
            first_idx = mid;
            j = mid - 1;  // procura mais à esquerda
        } else {
            i = mid + 1;
        }
    }
    
    return first_idx;
}

// binary search para encontrar último voo <= end_date
static int find_last_flight_in_range(GPtrArray* flights, int end_date, int start_idx) {
    if (!flights || flights->len == 0 || start_idx == -1) return -1;
    
    int i = start_idx;
    int j = flights->len - 1;
    int last_idx = -1;
    
    while (i <= j) {
        int mid = i + (j - i) / 2;
        Flight* flight = g_ptr_array_index(flights, mid);
        int flight_date = get_date_part(get_flight_actual_departure(flight));
        
        if (flight_date <= end_date) {
            last_idx = mid;
            i = mid + 1;  // procura mais à direita
        } else {
            j = mid - 1;
        }
    }
    
    return last_idx;
}

int airport_catalog_count_flights_in_range(const AirportData* data, int start_date, int end_date) {
    if (!data || !data->departing_flights) return 0;
    
    GPtrArray* flights = data->departing_flights;
    
    int first_idx = find_first_flight_in_range(flights, start_date); // binary search para encontrar limites
    
    if (first_idx == -1) return 0;  // nenhum voo >= start_date
    
    int last_idx = find_last_flight_in_range(flights, end_date, first_idx);
    
    if (last_idx == -1 || last_idx < first_idx) return 0;
    
    return last_idx - first_idx + 1;
}

// getters

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

int airport_get_arrival_count(AirportCatalog* manager, char* code) {
    if (manager == NULL || code == NULL) return 0;

    AirportData* data = g_hash_table_lookup(manager->airport_data_by_code, code);
    if (data == NULL) return 0;

    else return data->arrival_count;
}

int airport_get_departure_count(AirportCatalog* manager, char* code) {
    if (manager == NULL || code == NULL) return 0;

    AirportData* data = g_hash_table_lookup(manager->airport_data_by_code, code);
    if (data == NULL) return 0;

    else return data->departure_count;
}