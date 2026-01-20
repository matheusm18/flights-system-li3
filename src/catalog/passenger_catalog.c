#include "catalog/passenger_catalog.h"
#include <stdlib.h>
#include <string.h>
#include "utils/validation_utils.h"
#include "utils/query4_utils.h"
#include "entities/passenger.h"

struct weekly_top {
    char* week_key;
    char* top_ids[10];
    int count;
};

struct passenger_spend{
    char* id;
    double value;
};

struct passenger_catalog {
    GHashTable* passengers_by_document_number; 

    GHashTable* weekly_stats; // query 4
    WeeklyTop** timeline; // query 4
    int timeline_size;
};

PassengerCatalog* passenger_catalog_create() {
    PassengerCatalog* manager = malloc(sizeof(PassengerCatalog));
    manager->passengers_by_document_number = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify) destroy_passenger);

    manager->weekly_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);
    manager->timeline = NULL;
    manager->timeline_size = 0;

    return manager;
}

void passenger_catalog_destroy(PassengerCatalog* manager) {
    if (!manager) return;
    
    g_hash_table_destroy(manager->passengers_by_document_number);

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

void passenger_catalog_add(PassengerCatalog* manager, Passenger* passenger) {
    if (!manager || !passenger) return;
    
    int id = get_passenger_dnumber(passenger);
    g_hash_table_insert(manager->passengers_by_document_number, GINT_TO_POINTER(id), passenger); 
    
}

void passenger_catalog_add_price_increment (PassengerCatalog* manager, const char* weekID, const char* passenger_id, double price) {
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

static int compare_passengers(const void* a, const void* b) {
    PassengerSpend* p1 = (PassengerSpend*)a;
    PassengerSpend* p2 = (PassengerSpend*)b;
    
    if (p1->value > p2->value) return -1;
    if (p1->value < p2->value) return 1;
    return strcmp(p1->id, p2->id); 
}

static int compare_weeks(const void* a, const void* b) {
    const WeeklyTop *w1 = *(const WeeklyTop **)a;
    const WeeklyTop *w2 = *(const WeeklyTop **)b;
    return strcmp(w1->week_key, w2->week_key);
}

static int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void passenger_catalog_prepare_query4 (PassengerCatalog* manager) {
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

        for(int i = 0; i < 10; i++){
            week_stats->top_ids[i] = NULL;
        }

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

char* passenger_catalog_get_top_passenger_in_period(PassengerCatalog* manager, char* begin_date, char* end_date, int* out_count) {
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

int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number) {
    if (!manager) return 0;

    return g_hash_table_contains(manager->passengers_by_document_number, GINT_TO_POINTER(document_number));
}

Passenger* get_passenger_by_dnumber(PassengerCatalog* manager, int document_number) {
    if (!manager) return NULL;

    return g_hash_table_lookup(manager->passengers_by_document_number, GINT_TO_POINTER(document_number));
}

