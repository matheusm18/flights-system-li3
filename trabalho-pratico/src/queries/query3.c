/* src/queries/query3.c */
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queries/query3.h"
#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "entities/flight.h"
#include "entities/airport.h"
#include "utils/date.h"

/* compara DateTime: <0 se a<b, 0 se igual, >0 se a>b */
static int compare_datetime(const DateTime* a, const DateTime* b) {
    if (a == b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    if (a->date_part.year != b->date_part.year) return a->date_part.year - b->date_part.year;
    if (a->date_part.month != b->date_part.month) return a->date_part.month - b->date_part.month;
    if (a->date_part.day != b->date_part.day) return a->date_part.day - b->date_part.day;
    if (a->hour != b->hour) return a->hour - b->hour;
    return a->minute - b->minute;
}

static int is_cancelled(const char* status) {
    if (!status) return 0;
    return g_ascii_strcasecmp(status, "cancelled") == 0;
}

typedef struct {
    DateTime* start;
    DateTime* end;
    GHashTable* counts; /* key = strdup(origin) -> value = int* */
} Ctx;

/* callback para cada entry na hashtable de flights */
static void count_cb(gpointer key, gpointer value, gpointer user_data) {
    (void) key;
    Ctx* ctx = (Ctx*) user_data;
    Flight* f = (Flight*) value;
    if (!f) return;

    DateTime* actual = get_flight_actual_departure(f);
    const char* origin = get_origin_flight(f);
    const char* status = get_flight_status(f);

    if (!origin || !actual) return;
    if (is_cancelled(status)) return;
    if (compare_datetime(actual, ctx->start) < 0) return;
    if (compare_datetime(actual, ctx->end) > 0) return;

    gpointer v = g_hash_table_lookup(ctx->counts, origin);
    if (!v) {
        char* kdup = g_strdup(origin);
        int* cnt = g_new(int, 1);
        *cnt = 1;
        g_hash_table_insert(ctx->counts, kdup, cnt);
    } else {
        int* cnt = (int*) v;
        (*cnt)++;
    }
}

void execute_query3(FlightCatalog* flight_manager, AirportCatalog* airport_manager, const char* start_date_str, const char* end_date_str, const char* output_path) {
    if (!flight_manager || !start_date_str || !end_date_str || !output_path) return;

    DateTime* start = datetime_create_from_string(start_date_str);
    DateTime* end = datetime_create_from_string(end_date_str);

    if (!start || !end) {
        FILE* out = fopen(output_path, "w");
        if (out) { fprintf(out, "\n"); fclose(out); }
        if (start) datetime_destroy(start);
        if (end) datetime_destroy(end);
        return;
    }

    GHashTable* counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    Ctx ctx = { .start = start, .end = end, .counts = counts };

    GHashTable* flights = get_flight_catalog(flight_manager);
    if (flights != NULL) {
        g_hash_table_foreach(flights, count_cb, &ctx);
    } else {
        FILE* out = fopen(output_path, "w");
        if (out) { fprintf(out, "\n"); fclose(out); }
        g_hash_table_destroy(counts);
        datetime_destroy(start); datetime_destroy(end);
        return;
    }

    const char* best_code = NULL;
    int best_count = 0;

    GHashTableIter iter;
    gpointer k, v;
    g_hash_table_iter_init(&iter, counts);
    while (g_hash_table_iter_next(&iter, &k, &v)) {
        const char* code = (const char*) k;
        int cnt = *((int*) v);

        if (!best_code || cnt > best_count || (cnt == best_count && strcmp(code, best_code) < 0)) {
            best_code = code;
            best_count = cnt;
        }
    }

    FILE* out = fopen(output_path, "w");
    if (!out) {
        g_hash_table_destroy(counts);
        datetime_destroy(start); datetime_destroy(end);
        return;
    }

    if (!best_code) {
        fprintf(out, "\n");
    } else {
        const char* name = "";
        const char* city = "";
        const char* country = "";

        Airport* a = get_airport_by_code(airport_manager, best_code);
        if (a) {
            name = get_airport_name(a);
            city = get_airport_city(a);
            country = get_airport_country(a);
        }

        fprintf(out, "%s, %s, %s, %s, %d\n", best_code, name, city, country, best_count);
    }

    fclose(out);
    g_hash_table_destroy(counts);
    datetime_destroy(start);
    datetime_destroy(end);
}
