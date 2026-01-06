#include "catalog/reservation_catalog.h"
#include "entities/reservation.h"
#include "utils/validation_utils.h"

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Estrutura:
 *
 * reservations_by_id:
 *   reservation_id -> Reservation*
 *
 * nationality_stats:
 *   nationality (char*)
 *      -> GHashTable*
 *            airport_id (char*) -> int*
 */

struct reservation_stats_iterator {
    GHashTableIter iter;
};

struct reservation_catalog {
    GHashTable* reservation_ids; // dado a chave (reservation_id) devolve o proprio id, útil para ver se existe
    GHashTable* nationality_stats; // query 6
};

ReservationCatalog* reservation_catalog_create() {
    ReservationCatalog* manager = malloc(sizeof(ReservationCatalog));
    if (!manager) return NULL;

    manager->reservation_ids = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    manager->nationality_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);

    return manager;
}

void reservation_catalog_destroy(ReservationCatalog* manager) {
    if (!manager) return;

    g_hash_table_destroy(manager->reservation_ids);
    g_hash_table_destroy(manager->nationality_stats);

    free(manager);
}

void reservation_catalog_add(ReservationCatalog* manager, char* reservation_id) {
    if (!manager || !reservation_id) return;

    g_hash_table_add(manager->reservation_ids, strdup(reservation_id));
}

void reservation_catalog_add_nationality_increment (ReservationCatalog* manager, const char* nationality, const char* airport_id) {
    if (!manager || !nationality || !airport_id) return;

    // obter mapa de aeroportos de uma nacionalidade
    GHashTable* airport_map = g_hash_table_lookup(manager->nationality_stats, nationality);

    // se não existir, crio
    if (!airport_map) {
        airport_map = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        g_hash_table_insert(manager->nationality_stats, g_strdup(nationality), airport_map);
    }

    // obter contador do aeroporto*
    int* count = g_hash_table_lookup(airport_map, airport_id);

    // se não existir, crio contador
    if (!count) {
        count = malloc(sizeof(int));
        if (!count) return;

        *count = 0;
        g_hash_table_insert(airport_map, g_strdup(airport_id), count);
    }

    (*count)++;
}

// cria um iterador para percorrer os aeroportos de uma dada nacionalidade (query 6)
ReservationStatsIter* reservation_catalog_create_stats_iter(ReservationCatalog* manager, const char* nationality) {
    if (!manager || !nationality) return NULL;

    GHashTable* airport_map = g_hash_table_lookup(manager->nationality_stats, nationality);

    if (!airport_map) return NULL; // nacionalidade não tem dados

    ReservationStatsIter* it = malloc(sizeof(ReservationStatsIter));
    if (!it) return NULL;

    g_hash_table_iter_init(&(it->iter), airport_map);
    return it;
}

// avança para o próximo par (airport_id, count)
int reservation_catalog_stats_iter_next(ReservationStatsIter* it, const char** airport_id, int* count) {
    if (!it) return 0;

    gpointer key, value;
    if (g_hash_table_iter_next(&(it->iter), &key, &value)) {
        if (airport_id) *airport_id = (const char*)key; // para garantir que a query não altera o valor
        if (count) *count = *(int*)value;
        return 1; // tem mais elementos
    }
    
    return 0; // chegou ao fim
}

void reservation_catalog_stats_iter_destroy(ReservationStatsIter* it) {
    if (it) free(it);
}

bool reservation_exists(ReservationCatalog* manager, char* reservation_id) {
    if (!manager || !reservation_id) return false;
    
    return g_hash_table_contains(manager->reservation_ids, reservation_id);
}