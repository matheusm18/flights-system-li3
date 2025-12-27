#include "catalog/reservation_catalog.h"
#include "entities/reservation.h"

#include <glib.h>
#include <stdlib.h>
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

struct reservation_catalog {
    GHashTable* reservations_by_id;
    GHashTable* nationality_stats;
};

ReservationCatalog* reservation_catalog_create() {
    ReservationCatalog* manager = malloc(sizeof(ReservationCatalog));
    if (!manager) return NULL;

    manager->reservations_by_id = g_hash_table_new_full(g_str_hash, g_str_equal,g_free, (GDestroyNotify) destroy_reservation);

    manager->nationality_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);

    return manager;
}

void reservation_catalog_destroy(ReservationCatalog* manager) {
    if (!manager) return;

    g_hash_table_destroy(manager->reservations_by_id);
    g_hash_table_destroy(manager->nationality_stats);

    free(manager);
}

void reservation_catalog_add(ReservationCatalog* manager, Reservation* reservation) {
    if (!manager || !reservation) return;

    char* identifier = get_reservation_identifier(reservation);
    if (!identifier) return;

    g_hash_table_insert(manager->reservations_by_id, strdup(identifier), reservation);

    free(identifier);
}

Reservation* get_reservation_by_id (ReservationCatalog* manager, char* reservation_id) {
    if (!manager || !reservation_id) return NULL;

    return g_hash_table_lookup(manager->reservations_by_id, reservation_id);
}


void reservation_catalog_add_nationality_increment (ReservationCatalog* manager, const char* nationality, const char* airport_id) {
    if (!manager || !nationality || !airport_id) return;

    /*obter mapa de aeroportos de uma nacionalidade*/
    GHashTable* airport_map = g_hash_table_lookup(manager->nationality_stats, nationality);

    /*se não existir, crio*/
    if (!airport_map) {
        airport_map = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        g_hash_table_insert(manager->nationality_stats, g_strdup(nationality), airport_map);
    }

    /*obter contador do aeroporto*/
    int* count = g_hash_table_lookup(airport_map, airport_id);

    /*se não existir, crio contador*/
    if (!count) {
        count = malloc(sizeof(int));
        if (!count) return;

        *count = 0;
        g_hash_table_insert(airport_map, g_strdup(airport_id), count);
    }

    (*count)++;
}

GHashTable* reservation_catalog_get_nationality_map (ReservationCatalog* manager, const char* nationality) {
    if (!manager || !nationality) return NULL;

    return g_hash_table_lookup(manager->nationality_stats, nationality);
}
