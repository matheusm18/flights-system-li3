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
    
    struct tm time_struct = {0}; // inicializa a estrutura tm de time.h tudo a zeros
    time_struct.tm_year = year - 1900; // tm_year conta dados desde 1900
    time_struct.tm_mon = month - 1; // define o mês, como vai de 0 a 11 subtraimos 1
    time_struct.tm_mday = day; // define o dia do mês
    time_struct.tm_isdst = -1;
    
    mktime(&time_struct);
    
    int day_of_year = time_struct.tm_yday; // 0-364/365

/*
para calcular o número da semana, tendo em conta que elas começam aos domingos
e sabendo que a semana 1 vai começar no primeiro domingo do ano, temos de descobrir o primeiro domingo do ano
para isso calculamos em que dia da semana foi 1 de janeiro.

*/
    
    struct tm jan1 = {0};
    jan1.tm_year = year - 1900;
    jan1.tm_mon = 0;
    jan1.tm_mday = 1;
    jan1.tm_isdst = -1;
    mktime(&jan1);
    
    int jan1_day_of_week = jan1.tm_wday;  
    
    int days_to_first_sunday;

    if(jan1_day_of_week == 0) {
        days_to_first_sunday = 0;
    } else {
        days_to_first_sunday = 7 - jan1_day_of_week;
    }  
    
    
    // calcular semana
    int days_since_first_sunday = day_of_year - days_to_first_sunday; // quantos dias é que já se passaram desde o primeiro domingo
    
    if (days_since_first_sunday >= 0) {
        wn->year = year;
        wn->week = (days_since_first_sunday / 7) + 1;
    } else { // data antes do primeiro domingo, pertence à ultima semana do ano anterior
        int previous_year = year - 1;

        // descobrir o 31 de dezembro do ano anterior
        struct tm dec31_prev = {0};
        dec31_prev.tm_year = previous_year - 1900;
        dec31_prev.tm_mon = 11;  // Dezembro
        dec31_prev.tm_mday = 31;
        dec31_prev.tm_isdst = -1;
        mktime(&dec31_prev);

        // calcular quantos dias desde o último domingo do ano anterior
        int jan1_day_prev = (dec31_prev.tm_wday + 1) % 7; // domingo = 0
        int days_since_last_sunday = dec31_prev.tm_yday - jan1_day_prev;
        int last_week_number = (days_since_last_sunday / 7) + 1;

        wn->year = previous_year;
        wn->week = last_week_number;
    }
    
    return wn;
}

// criar uma chave única para cada semana exemplo: 2024-W01
char* create_week_key(int year, int week) {
    char *key = malloc(20);
    snprintf(key, 20, "%d-W%02d", year, week);
    return key;
}




