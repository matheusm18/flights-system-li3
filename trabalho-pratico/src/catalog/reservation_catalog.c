#include "catalog/reservation_catalog.h"
#include "entities/reservation.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct reservation_catalog {
    GHashTable* reservations_by_id;
};

ReservationCatalog* reservation_catalog_create() {
    ReservationCatalog* manager = malloc(sizeof(ReservationCatalog));
    manager->reservations_by_id = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_reservation);

    return manager;
}

void reservation_catalog_destroy(ReservationCatalog* manager) {
    if (manager != NULL) {
        g_hash_table_destroy(manager->reservations_by_id);
        free(manager);
    }
}

void reservation_catalog_add(ReservationCatalog* manager, Reservation* reservation) {
    if (!manager || !reservation) return;
    
    char* identifier = get_reservation_identifier(reservation);
    if (identifier != NULL) {
        g_hash_table_insert(manager->reservations_by_id, g_strdup(identifier), reservation);
        free(identifier);
    }
}

Reservation* get_reservation_by_id(ReservationCatalog* manager, char* reservation_id) {
    if (!manager || !reservation_id) return NULL;

    return g_hash_table_lookup(manager->reservations_by_id, reservation_id);
}