#include "catalog/passenger_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/passenger.h"

struct passenger_catalog {
    GHashTable* passengers_by_document;
};

PassengerCatalog* passenger_catalog_create() {
    PassengerCatalog* manager = malloc(sizeof(PassengerCatalog));
    manager->passengers_by_document = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_passenger);

    return manager;
}

void passenger_catalog_destroy(PassengerCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->passengers_by_document);
        free(manager);
    }
}

void passenger_catalog_add(PassengerCatalog* manager, Passenger* passenger) {
    if (!manager || !passenger) return;
    
    const char* code = get_passenger_dnumber(passenger);
    if (code != NULL) {
        g_hash_table_insert(manager->passengers_by_document, g_strdup(code), passenger); 
    }
}

Passenger* get_passenger_by_dnumber(PassengerCatalog* manager, const char* dnumber) {
    if (manager == NULL || dnumber == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->passengers_by_document, dnumber); 
}

