#include "catalog/reservation_catalog.h"
#include "entities/reservation.h"
#include "utils/utils_validation.h"
#include "utils/query4_utils.h"

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

    WeeklyTop** timeline; // array ordenado
    int timeline_size; // tamanho do array
};

ReservationCatalog* reservation_catalog_create() {
    ReservationCatalog* manager = malloc(sizeof(ReservationCatalog));
    if (!manager) return NULL;

    manager->reservations_by_id = g_hash_table_new_full(g_str_hash, g_str_equal,g_free, (GDestroyNotify) destroy_reservation);

    manager->nationality_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);
    manager->weekly_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) g_hash_table_destroy);

    manager->timeline = NULL;
    manager->timeline_size = 0;

    return manager;
}

void reservation_catalog_destroy(ReservationCatalog* manager) {
    if (!manager) return;

    g_hash_table_destroy(manager->reservations_by_id);
    g_hash_table_destroy(manager->nationality_stats);

    if (manager->weekly_stats) g_hash_table_destroy(manager->weekly_stats);

    if (manager->timeline) {
        for (int i = 0; i < manager->timeline_size; i++) {
            // liberta a string da chave
            free(manager->timeline[i]->week_key);
            // liberta as strings dos ids dos passageiros
            for (int j = 0; j < manager->timeline[i]->count; j++) {
                free(manager->timeline[i]->top_ids[j]);
            }
            // liberta a struct da semana
            free(manager->timeline[i]);
        }
        // liberta o array principal
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
    WeeklyTop* w1 = *(WeeklyTop**)a;
    WeeklyTop* w2 = *(WeeklyTop**)b;
    return strcmp(w1->week_key, w2->week_key);
}

void reservation_catalog_prepare_metrics(ReservationCatalog* manager) {
    if (!manager || !manager->weekly_stats) return;

    // alocar o array da timeline
    guint num_weeks = g_hash_table_size(manager->weekly_stats);
    manager->timeline = malloc(sizeof(WeeklyTop*) * num_weeks);
    manager->timeline_size = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, manager->weekly_stats);

    // iterar sobre cada semana da hash table
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* week_str = (char*)key;
        GHashTable* week_data = (GHashTable*)value;

        // criar struct de resumo da semana
        WeeklyTop* week_stats = malloc(sizeof(WeeklyTop));
        week_stats->week_key = strdup(week_str);
        week_stats->count = 0;

        // passar dados da hash para array temporário para ordenar
        guint num_p = g_hash_table_size(week_data);
        PassengerSpend* temp_list = malloc(sizeof(PassengerSpend) * num_p);
        int idx = 0;

        GHashTableIter inner_iter;
        gpointer inner_key, inner_val;
        g_hash_table_iter_init(&inner_iter, week_data);

        while (g_hash_table_iter_next(&inner_iter, &inner_key, &inner_val)) {
            temp_list[idx].id = (char*)inner_key;
            temp_list[idx].value = *(double*)inner_val;
            idx++;
        }

        // ordenar para encontrar os vencedores
        qsort(temp_list, num_p, sizeof(PassengerSpend), compare_passengers);

        // guardar apenas o TOP 10
        int limit = (num_p < 10) ? num_p : 10;
        for (int i = 0; i < limit; i++) {
            week_stats->top_ids[i] = strdup(temp_list[i].id);
        }
        week_stats->count = limit;

        manager->timeline[manager->timeline_size++] = week_stats;
        free(temp_list);
    }

    // 3. ordenar a timeline por data (WeekKey)
    qsort(manager->timeline, manager->timeline_size, sizeof(WeeklyTop*), compare_weeks);

    // 4. libertar a Hash Table (já não precisamos dela)
    g_hash_table_destroy(manager->weekly_stats);
    manager->weekly_stats = NULL; 
}

char* reservation_catalog_get_top_passenger_in_period(ReservationCatalog* manager, char* begin_date, char* end_date, int* out_count) {
    if (out_count) *out_count = 0;
    
    if (!manager) return NULL;

    char* filter_start_week = NULL;
    char* filter_end_week = NULL;

    char start_week_buf[20] = {0};
    char end_week_buf[20]   = {0};

    if (begin_date) {
        date_to_week_key_buf(string_to_date(begin_date), start_week_buf);
        filter_start_week = start_week_buf;
    }

    if (end_date) {
        date_to_week_key_buf(string_to_date(end_date), end_week_buf);
        filter_end_week = end_week_buf;
    }

    GHashTable* passenger_frequency_map = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);

    char* best_passenger = NULL;
    int max_frequency = 0;

    // iterar pela timeline (que já está ordenada alfabeticamente/cronologicamente)
    for (int i = 0; i < manager->timeline_size; i++) {
        WeeklyTop* week_stats = manager->timeline[i];

        // se temos filter_start_week e a semana atual é "menor", avançamos
        if (filter_start_week && strcmp(week_stats->week_key, filter_start_week) < 0) {
            continue;
        }

        // se temos filter_end_week e a semana atual é "maior", paramos (break)
        if (filter_end_week && strcmp(week_stats->week_key, filter_end_week) > 0) {
            break;
        }

        // contar os passageiros desta semana válida
        for (int j = 0; j < week_stats->count; j++) {
            char* p_id = week_stats->top_ids[j]; // passenger_id
            
            int* frequency_ptr = g_hash_table_lookup(passenger_frequency_map, p_id); // aponta para a frequencia atual do passageiro
            
            if (!frequency_ptr) {
                frequency_ptr = malloc(sizeof(int));
                *frequency_ptr = 1;
                // usamos o passenger_id que já está na memória do timeline como chave
                g_hash_table_insert(passenger_frequency_map, p_id, frequency_ptr); 
            } else {
                (*frequency_ptr)++;
            }

            int current_val = *frequency_ptr;

            // verificar se é o novo líder
            if (current_val > max_frequency) {
                max_frequency = current_val;
                best_passenger = p_id;
            } 
            else if (current_val == max_frequency) {
                // desempate: identificador mais baixo
                if (best_passenger == NULL || strcmp(p_id, best_passenger) < 0) {
                    best_passenger = p_id;
                }
            }
        }
    }

    if (out_count) *out_count = max_frequency;

    char* result = NULL;
    if (best_passenger) {
        result = strdup(best_passenger);
    }

    g_hash_table_destroy(passenger_frequency_map);
    return result;
}