#include "catalog/passenger_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/passenger.h"


struct passenger_catalog {
    GHashTable* document_numbers; 
};

PassengerCatalog* passenger_catalog_create() {
    PassengerCatalog* manager = malloc(sizeof(PassengerCatalog));
    manager->document_numbers = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);

    return manager;
}

void passenger_catalog_destroy(PassengerCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->document_numbers);
        free(manager);
    }
}

void passenger_catalog_add(PassengerCatalog* manager, int document_number) {
    if (!manager) return;
    
    g_hash_table_insert(manager->document_numbers, GINT_TO_POINTER(document_number), NULL); 
    
}

int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number) {
    return g_hash_table_contains(manager->document_numbers, GINT_TO_POINTER(document_number));
}