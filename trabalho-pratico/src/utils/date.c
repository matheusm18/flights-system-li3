#include "utils/date.h"
#include <stdio.h>
#include <stdlib.h>

Date* date_create(int year, int month, int day) {
    Date* date = malloc(sizeof(Date));
    if (date == NULL) return NULL;

    date->year = year;
    date->month = month; 
    date->day = day;
    
    return date;
}

Date* date_create_from_string(const char* str) {
    if (!str) return NULL;

    Date* date = malloc(sizeof(Date));
    if (!date) return NULL;

    // Formato: "YYYY-MM-DD"
    int scanned = sscanf(str, "%d-%d-%d", &date->year, &date->month, &date->day);

    if (scanned != 3) {
        free(date);
        return NULL;
    }

    // Validação básica
    if (date->month < 1 || date->month > 12 || date->day < 1 || date->day > 31) {
        free(date);
        return NULL;
    }

    return date;
}

int date_compare(const Date* date1, const Date* date2) {
    if (date1->year != date2->year) return date1->year - date2->year;
    if (date1->month != date2->month) return date1->month - date2->month;
    return date1->day - date2->day;
}


DateTime* datetime_create(int year, int month, int day, int hour, int minute) {
    struct datetime* dt = malloc(sizeof(struct datetime));
    if (!dt) return NULL;
    dt->date_part.year = year;
    dt->date_part.month = month;
    dt->date_part.day = day;
    dt->hour = hour;
    dt->minute = minute;
    return dt;
}

DateTime* datetime_create_from_string(const char* str) {
    if (!str) return NULL;

    struct datetime* dt = malloc(sizeof(struct datetime));
    if (!dt) return NULL;

    int scanned = sscanf(str, "%d-%d-%d %d:%d", &dt->date_part.year, &dt->date_part.month, &dt->date_part.day, &dt->hour, &dt->minute);

    if (scanned != 5) {
        free(dt);
        return NULL;
    }

    return dt;
}

// compara DateTime: <0 se a<b, 0 se igual, >0 se a>b
int compare_datetime(const DateTime* a, const DateTime* b) {
    if (a == b) return 0;
    if (!a) return -1;
    if (!b) return 1;

    if (a->date_part.year != b->date_part.year) return a->date_part.year - b->date_part.year;
    if (a->date_part.month != b->date_part.month) return a->date_part.month - b->date_part.month;
    if (a->date_part.day != b->date_part.day) return a->date_part.day - b->date_part.day;
    if (a->hour != b->hour) return a->hour - b->hour;

    return a->minute - b->minute;
}

void date_destroy(Date* date) {
    free(date);
}

void datetime_destroy(DateTime* datetime) {
    free(datetime);
}

// getters:

int date_get_year(const Date* d) {
    return d->year;
}

int date_get_month(const Date* d) {
    return d->month;
}

int date_get_day(const Date* d) {
    return d->day;
}
