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

void date_destroy(Date* date) {
    free(date);
}

int date_compare(const Date* date1, const Date* date2) {
    return 0;
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

struct datetime* datetime_create(int year, int month, int day, int hour, int minute) {
    struct datetime* dt = malloc(sizeof(struct datetime));
    if (!dt) return NULL;
    dt->date_part.year = year;
    dt->date_part.month = month;
    dt->date_part.day = day;
    dt->hour = hour;
    dt->minute = minute;
    return dt;
}

struct datetime* datetime_create_from_string(const char* str) {
    if (!str) return NULL;

    struct datetime* dt = malloc(sizeof(struct datetime));
    if (!dt) return NULL;

    // Inicializa valores
    dt->date_part.year = 0;
    dt->date_part.month = 0;
    dt->date_part.day = 0;
    dt->hour = 0;
    dt->minute = 0;

    // Usa sscanf para extrair os valores
    int scanned = sscanf(str, "%d-%d-%d %d:%d",
                         &dt->date_part.year,
                         &dt->date_part.month,
                         &dt->date_part.day,
                         &dt->hour,
                         &dt->minute);

    // Verifica se todos os 5 valores foram lidos
    if (scanned != 5) {
        free(dt);
        return NULL; // Formato inválido
    }

    return dt;
}

void datetime_destroy(DateTime* datetime) {
    free(datetime);
}