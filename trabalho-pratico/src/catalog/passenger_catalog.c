#include "catalog/passenger_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/passenger.h"

struct passenger_catalog {
    GHashTable* passengers_by_document_number; 
};

PassengerCatalog* passenger_catalog_create() {
    PassengerCatalog* manager = malloc(sizeof(PassengerCatalog));
    manager->passengers_by_document_number = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify) destroy_passenger);

    return manager;
}

void passenger_catalog_destroy(PassengerCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->passengers_by_document_number);
        free(manager);
    }
}

void passenger_catalog_add(PassengerCatalog* manager, Passenger* passenger) {
    if (!manager || !passenger) return;
    
    int id = get_passenger_dnumber(passenger);
    g_hash_table_insert(manager->passengers_by_document_number, GINT_TO_POINTER(id), passenger); 
    
}

int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number) {
    if (!manager) return 0;

    return g_hash_table_contains(manager->passengers_by_document_number, GINT_TO_POINTER(document_number));
}

Passenger* get_passenger_by_dnumber(PassengerCatalog* manager, int document_number) {
    if (!manager) return NULL;

    return g_hash_table_lookup(manager->passengers_by_document_number, GINT_TO_POINTER(document_number));
}

