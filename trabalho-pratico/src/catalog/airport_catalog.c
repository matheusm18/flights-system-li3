#include "catalog/airport_catalog.h"
#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>

struct airport_catalog {
    GHashTable* airports_by_code; // vai ser a responsável por liberar os ponteiros dos aeroportos quando for destruida
    GPtrArray* all_airports; // array dinamico
};

AirportCatalog* airport_catalog_create() {
    AirportCatalog* manager = malloc(sizeof(AirportCatalog));
    manager->airports_by_code = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_airport);
    manager->all_airports = g_ptr_array_new(); // deixar uma função separada para criar o array dinamico?

    return manager;
}

void airport_catalog_destroy(AirportCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->airports_by_code);
        g_ptr_array_free(manager->all_airports, TRUE); // não faria sentido ser false? o resto ja foi libertado na anterior?
        free(manager);
    }
}

void airport_catalog_add(AirportCatalog* manager, Airport* airport) {
    if (!manager || !airport) return;
    
    const char* code = airport_get_code(airport);
    if (code != NULL) {
        g_hash_table_insert(manager->airports_by_code, (gpointer)code, airport); // não seria melhor usar g_strdup?
        g_ptr_array_add(manager->all_airports, airport); // deixar uma função separada para adicionar no array?
    }
}

Airport* airport_catalog_get_by_code(AirportCatalog* manager, const char* code) {
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

GList* airport_catalog_get_all_airports(AirportCatalog* manager) {
    if (manager == NULL) return NULL;
    
    GList* list = NULL;
    for (guint i = 0; i < manager->all_airports->len; i++) {
        Airport* airport = g_ptr_array_index(manager->all_airports, i);
        list = g_list_append(list, airport);
    }
    return list;
}