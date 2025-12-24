#include "utils/date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int string_to_date(const char* str) {
    if (!str || strcmp(str, "N/A") == 0) return -1; ///< Considerar "N/A" como -1

    int year = (str[0]-'0')*1000 + (str[1]-'0')*100 + (str[2]-'0')*10 + (str[3]-'0');
    int month = (str[5]-'0')*10 + (str[6]-'0');
    int day = (str[8]-'0')*10 + (str[9]-'0');

    return year * 10000 + month * 100 + day;
}

long string_to_datetime(const char* str) {
    if (!str || strcmp(str, "N/A") == 0) return -1;

    int y = (str[0]-'0')*1000 + (str[1]-'0')*100 + (str[2]-'0')*10 + (str[3]-'0');
    int m = (str[5]-'0')*10 + (str[6]-'0');
    int d = (str[8]-'0')*10 + (str[9]-'0');
    int h = (str[11]-'0')*10 + (str[12]-'0');
    int min = (str[14]-'0')*10 + (str[15]-'0');
    
    return (long)y*100000000L + m*1000000L + d*10000L + h*100 + min; ///< O 'L' força o cálculo a ser feito em long (evita overflow)
}

int get_date_part(long datetime) {
    return (int)(datetime / 10000);
}

int compare_dates(int date1, int date2) {
    return date1 - date2;
}

int compare_datetimes(long dt1, long dt2) {
    if (dt1 < dt2) return -1;
    if (dt1 > dt2) return 1;
    return 0;
}


int get_flight_delay(long actual_departure, long expected_departure) {
    if (actual_departure <= 0 || expected_departure <= 0) return 0;

    struct tm t_actual_departure = {0}, t_expected_departure = {0};

    t_actual_departure.tm_min  = actual_departure % 100;
    t_actual_departure.tm_hour = (actual_departure / 100) % 100;
    t_actual_departure.tm_mday = (actual_departure / 10000) % 100;
    t_actual_departure.tm_mon  = ((actual_departure / 1000000) % 100) - 1;
    t_actual_departure.tm_year = (actual_departure / 100000000L) - 1900;
    t_actual_departure.tm_isdst = -1;

    t_expected_departure.tm_min  = expected_departure % 100;
    t_expected_departure.tm_hour = (expected_departure / 100) % 100;
    t_expected_departure.tm_mday = (expected_departure / 10000) % 100;
    t_expected_departure.tm_mon  = ((expected_departure / 1000000) % 100) - 1;
    t_expected_departure.tm_year = (expected_departure / 100000000L) - 1900;
    t_expected_departure.tm_isdst = -1;

    // converter ambos para time_t (segundos)
    time_t s_actual_departure = mktime(&t_actual_departure);
    time_t s_expected_departure = mktime(&t_expected_departure);

    // diferença em segundos dividida por 60 (para dar em minutos)
    return (int)(difftime(s_actual_departure, s_expected_departure) / 60);
}