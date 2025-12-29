#include "catalog/reservation_catalog.h"
#include "entities/reservation.h"
#include "utils/utils_validation.h"
#include "utils/query4_utils.h"

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

struct weekly_top {
    char* week_key;
    char* top_ids[10];
    int count;
};

struct passenger_spend{
    char* id;
    double value;
};

struct reservation_stats_iterator {
    GHashTableIter iter;
};

struct reservation_catalog {
    GHashTable* reservations_by_id;
    GHashTable* nationality_stats;
    GHashTable* weekly_stats;

    WeeklyTop** timeline;
    int timeline_size;
};

ReservationCatalog* reservation_catalog_create() {
    ReservationCatalog* manager = malloc(sizeof(ReservationCatalog));
    if (!manager) return NULL;

    manager->reservations_by_id = g_hash_table_new_full(g_str_hash, g_str_equal,g_free, (GDestroyNotify) destroy_reservation);

    manager->nationality_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);
    manager->weekly_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);

    return manager;
}

void reservation_catalog_destroy(ReservationCatalog* manager) {
    if (!manager) return;

    g_hash_table_destroy(manager->reservations_by_id);
    g_hash_table_destroy(manager->nationality_stats);

    if (manager->weekly_stats) g_hash_table_destroy(manager->weekly_stats);

    if (manager->timeline) {
        for (int i = 0; i < manager->timeline_size; i++) {
            WeeklyTop *wt = manager->timeline[i];
            free(wt->week_key);
            for (int j = 0; j < wt->count; j++) {
                free(wt->top_ids[j]);
            }
            free(wt);
        }
        free(manager->timeline);
    }

    free(manager);
}

void reservation_catalog_add(ReservationCatalog* manager, Reservation* reservation) {
    if (!manager || !reservation) return;

    char* identifier = get_reservation_identifier(reservation);
    if (!identifier) return;

    g_hash_table_insert(manager->reservations_by_id, strdup(identifier), reservation);

    free(identifier);
}

void reservation_catalog_add_price_increment (ReservationCatalog* manager, const char* weekID, const char* passenger_id, double price) {
    if (!manager || !weekID || !passenger_id) return;

    // tentar encontrar a SEMANA (hash table que guarda a informação de todos os passageiros dessa semana weekID)
    GHashTable* week_map = g_hash_table_lookup(manager->weekly_stats, weekID);

    // se é a primeira venda desta semana, criamos e inserimos a 
    if (!week_map) {
        week_map = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free); // criar a dessa semana
        g_hash_table_insert(manager->weekly_stats, g_strdup(weekID), week_map);
    }

    double* current_total = g_hash_table_lookup(week_map, passenger_id);

    // se o passageiro não existe nesta semana, criamos a info
    if (!current_total) {
        current_total = malloc(sizeof(double));
        if (current_total) {
            *current_total = 0.0;
            g_hash_table_insert(week_map, g_strdup(passenger_id), current_total); // ver se strdup é necessario ou nao
        }
    }
    if (current_total) {
        *current_total += price;
    }
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

// avança para o próximo par (airport_id, count). retorna 1 se for para continuar o loop, 0 se for para terminar.
// os valores são retornados por referência nos argumentos 'airport_id' e 'count'.
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

Reservation* get_reservation_by_id (ReservationCatalog* manager, char* reservation_id) {
    if (!manager || !reservation_id) return NULL;

    return g_hash_table_lookup(manager->reservations_by_id, reservation_id);
}

int compare_passengers(const void* a, const void* b) {
    PassengerSpend* p1 = (PassengerSpend*)a;
    PassengerSpend* p2 = (PassengerSpend*)b;
    
    if (p1->value > p2->value) return -1;
    if (p1->value < p2->value) return 1;
    return strcmp(p1->id, p2->id); 
}

int compare_weeks(const void* a, const void* b) {
    const WeeklyTop *w1 = *(const WeeklyTop **)a;
    const WeeklyTop *w2 = *(const WeeklyTop **)b;
    return strcmp(w1->week_key, w2->week_key);
}

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void reservation_catalog_prepare_query4 (ReservationCatalog* manager) {
    if (!manager || !manager->weekly_stats) return;

    guint num_weeks = g_hash_table_size(manager->weekly_stats);
    if (num_weeks == 0) return;

    manager->timeline = malloc(sizeof(struct weekly_top*) * num_weeks);
    manager->timeline_size = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, manager->weekly_stats);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* week_str = (char*)key;
        GHashTable* week_data = (GHashTable*)value;

        struct weekly_top* week_stats = malloc(sizeof(struct weekly_top));
        week_stats->week_key = strdup(week_str);
        week_stats->count = 0;

        guint num_p = g_hash_table_size(week_data);
        if (num_p > 0) {
            struct passenger_spend* temp_list = malloc(sizeof(struct passenger_spend) * num_p);
            int idx = 0;

            GHashTableIter inner_iter;
            gpointer inner_key, inner_val;
            g_hash_table_iter_init(&inner_iter, week_data);

            while (g_hash_table_iter_next(&inner_iter, &inner_key, &inner_val)) {
                temp_list[idx].id = (char*)inner_key;
                temp_list[idx].value = *(double*)inner_val;
                idx++;
            }

            qsort(temp_list, num_p, sizeof(struct passenger_spend), compare_passengers);

            int limit = (num_p < 10) ? num_p : 10;
            for (int i = 0; i < limit; i++) {
                week_stats->top_ids[i] = strdup(temp_list[i].id);
            }
            week_stats->count = limit;
            free(temp_list);
        }

        manager->timeline[manager->timeline_size++] = week_stats;
    }

    qsort(manager->timeline, manager->timeline_size, sizeof(struct weekly_top*), compare_weeks);

    g_hash_table_destroy(manager->weekly_stats);
    manager->weekly_stats = NULL; 
}

char* reservation_catalog_get_top_passenger_in_period(ReservationCatalog* manager, char* begin_date, char* end_date, int* out_count) {
    if (out_count) *out_count = 0;
    if (!manager || !manager->timeline) return NULL;

    char start_key[40] = "";
    char end_key[40] = "9999-99-99"; // para o caso do null
    
    if (begin_date) {
        int d = string_to_date(begin_date);
        if (d > 0) date_to_week_key_buf(d, start_key);
    }
    if (end_date) {
        int d = string_to_date(end_date);
        if (d > 0) date_to_week_key_buf(d, end_key);
    }

    GPtrArray *candidates = g_ptr_array_new();

    for (int i = 0; i < manager->timeline_size; i++) {
        struct weekly_top* wt = manager->timeline[i];

        if (strcmp(wt->week_key, start_key) < 0) continue;
        if (strcmp(wt->week_key, end_key) > 0) break;

        for (int j = 0; j < wt->count; j++) {
            g_ptr_array_add(candidates, wt->top_ids[j]);
        }
    }

    if (candidates->len == 0) {
        g_ptr_array_free(candidates, TRUE);
        return NULL;
    }

    // ordena strings alfabeticamente
    qsort(candidates->pdata, candidates->len, sizeof(char*), compare_strings);

    char *winner = NULL;
    int max_streak = 0;
    
    char *current_id = g_ptr_array_index(candidates, 0);
    int current_streak = 1;

    winner = current_id;
    max_streak = 1;

    for (guint i = 1; i < candidates->len; i++) {
        char *next = g_ptr_array_index(candidates, i);
        
        if (strcmp(current_id, next) == 0) {
            current_streak++;
        } else {
            if (current_streak > max_streak) {
                max_streak = current_streak;
                winner = current_id;
            } else if (current_streak == max_streak) {
                // empate: id menor ganha
                if (strcmp(current_id, winner) < 0) winner = current_id;
            }
            current_id = next;
            current_streak = 1;
        }
    }

    if (current_streak > max_streak) {
        max_streak = current_streak;
        winner = current_id;
    } else if (current_streak == max_streak) {
        if (strcmp(current_id, winner) < 0) winner = current_id;
    }

    if (out_count) *out_count = max_streak;
    char *result_str = winner ? strdup(winner) : NULL;
    
    g_ptr_array_free(candidates, TRUE);
    return result_str;
}