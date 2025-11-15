#include "catalog/passenger_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "entities/passenger.h"

/* por enquanto em vez de guardar o passageiro inteiro, vamos guardar só o document number na hash table
   visto que é o que precisamos para validar as reservas e não precisamos de mais nada para já
*/

struct passenger_catalog {
    GHashTable* document_numbers; // set de ints
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

// neste caso para esta fase em vez de adicionar um passageiro, vamos adicionar o document number à hash table
void passenger_catalog_add(PassengerCatalog* manager, int document_number) {
    if (!manager) return;
    
    // NULL para o value, porque só queremos um set de chaves
    g_hash_table_insert(manager->document_numbers, GINT_TO_POINTER(document_number), NULL); 
    
}

// contains em vez de lookup visto que só temos chaves e não armazenamos nada associado
int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number) {
    return g_hash_table_contains(manager->document_numbers, GINT_TO_POINTER(document_number));
}