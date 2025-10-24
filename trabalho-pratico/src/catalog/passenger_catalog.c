#include "catalog/passenger_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/passenger.h"

struct passenger_catalog {
    GHashTable* passengers_by_document;
    GPtrArray* all_passengers;
};

PassengerCatalog* passenger_catalog_create() {
    PassengerCatalog* manager = malloc(sizeof(PassengerCatalog));
    manager->passengers_by_document = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_passenger);
    manager->all_passengers = g_ptr_array_new(); 

    return manager;
}

void pasenger_catalog_destroy(PassengerCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->passengers_by_document);
        g_ptr_array_free(manager->all_passengers, TRUE); 
        free(manager);
    }
}

void add_passenger_catalog(PassengerCatalog* manager, Passenger* passenger) {
    if (!manager || !passenger) return;
    
    const char* code = airget_passenger_dnumberport_get_code(passenger);
    if (code != NULL) {
        g_hash_table_insert(manager->passengers_by_document, (gpointer)code, passenger); 
        g_ptr_array_add(manager->all_passengers, passenger); 
    }
}

Passenger* get_passenger_by_dnumber(PassengerCatalog* manager, const char* dnumber) {
    if (manager == NULL || dnumber == NULL) {
        return NULL;
    }

    return g_hash_table_lookup(manager->passengers_by_document, dnumber); 
}

