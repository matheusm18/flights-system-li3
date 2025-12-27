#include "utils/query4_utils.h"
#include <glib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// estrutura auxiliar para representar uma semana
struct week_number {
    int year;
    int week;
};

WeekNumber* get_week_number(int year, int month, int day) {
    WeekNumber *wn = malloc(sizeof(WeekNumber));
    wn->year = year;

    struct tm time_struct = {0};
    time_struct.tm_year = year - 1900;
    time_struct.tm_mon = month - 1;
    time_struct.tm_mday = day;
    time_struct.tm_hour = 12; // IMPORTANTE: Meio-dia para evitar erros de fuso horário/DST
    time_struct.tm_isdst = -1;
    
    // O mktime preenche os campos tm_wday e tm_yday
    if (mktime(&time_struct) == -1) {
        // Tratamento de erro básico
        wn->week = 0;
        return wn;
    }
    
    // Descobrir em que dia da semana caiu 1 de Janeiro deste ano
    struct tm jan1 = {0};
    jan1.tm_year = year - 1900;
    jan1.tm_mon = 0;
    jan1.tm_mday = 1;
    jan1.tm_hour = 12;
    jan1.tm_isdst = -1;
    mktime(&jan1);
    
    // Algoritmo para Semana (Domingo a Sábado):
    // tm_yday: dia do ano (0 a 365)
    // jan1.tm_wday: dia da semana de 1 Jan (0=Dom, 1=Seg...)
    // A fórmula ajusta o dia do ano como se o ano começasse no Domingo anterior
    int week = (time_struct.tm_yday + jan1.tm_wday) / 7;
    
    // Somamos 1 porque a contagem começa em 0, e queremos Semana 1, Semana 2...
    wn->week = week + 1;
    
    return wn;
}

// criar uma chave única para cada semana exemplo: 2024-W01
char* create_week_key(int year, int week) {
    char *key = malloc(20);
    snprintf(key, 20, "%d-W%02d", year, week);
    return key;
}

char* date_to_week_key(int date) {
    if (date <= 0) return NULL;

    int year = date / 10000;           // 20250110 / 10000 = 2025
    int month = (date / 100) % 100;    // 20250110 / 100 = 202501 -> 202501 % 100 = 1
    int day = date % 100;              // 20250110 % 100 = 10

    WeekNumber* wn = get_week_number(year, month, day);
    char* key = create_week_key(wn->year, wn->week);
    
    free(wn);
    return key;
}


