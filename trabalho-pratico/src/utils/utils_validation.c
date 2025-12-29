#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <glib.h>

char* int_to_string(int value) {

    char buffer[32];

    // snprintf é seguro porque nunca escreve mais do que o tamanho do buffer
    snprintf(buffer, sizeof(buffer), "%d", value);

    // strdup aloca memória na heap exatamente do tamanho da string gerada
    return strdup(buffer);
}

char* double_to_string(double valor) {

    int tamanho = snprintf(NULL, 0, "%.3f", valor);
    
    char* str = malloc(tamanho + 1);
    
    if (str != NULL) {
        snprintf(str, tamanho + 1, "%.3f", valor);
    }
    
    return str;
}

char* date_to_string(int date) {
    if (date == -1) {
        return strdup("N/A");
    }

    int year = date / 10000;
    
    int month = (date % 10000) / 100;
    
    int day = date % 100;

    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);

    return strdup(buffer);
}


static inline int quick_atoi2(const char* s) {
    return (s[0] - '0') * 10 + (s[1] - '0');
}

static inline int quick_atoi4(const char* s) {
    return (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + (s[3] - '0');
}

bool validate_date(const char* date) {
    if (!date) return false; 
    if (date[4] != '-' || date[7] != '-') return false;


    const int indices[] = {0, 1, 2, 3, 5, 6, 8, 9};
    for (int k = 0; k < 8; k++) {
        int i = indices[k];
        if (date[i] < '0' || date[i] > '9') return false;
    }

    int year  = quick_atoi4(date);
    int month = quick_atoi2(&date[5]);
    int day   = quick_atoi2(&date[8]);

    if (month < 1 || month > 12 || day < 1 || day > 31) return false;

    int input_date = year * 10000 + month * 100 + day;
    const int current_date = 20250930; 
    
    return input_date <= current_date;
}

bool validate_time(const char* time) {
    if (!time || time[2] != ':') return false;

    if (time[0] < '0' || time[0] > '9' || time[1] < '0' || time[1] > '9' ||
        time[3] < '0' || time[3] > '9' || time[4] < '0' || time[4] > '9') return false;

    int hours   = quick_atoi2(time);
    int minutes = quick_atoi2(&time[3]);

    return (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);
}


bool validate_datetime(const char* datetime) {
    if (!datetime || datetime[0] == '\0') return false;
    
    if (datetime[0] == 'N' && datetime[1] == '/' && datetime[2] == 'A' && datetime[3] == '\0') {
        return true;
    }

    if (datetime[4] != '-' || datetime[7] != '-' || datetime[10] != ' ' || 
        datetime[13] != ':' || datetime[16] != '\0') {
        return false;
    }

    return validate_date(datetime) && validate_time(&datetime[11]);
}

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

bool string_to_bool(const char *str) {
    if (!str) return false;
    
    if (strcasecmp(str, "true") == 0) {
        return true;
    }
    return false;
}


bool validate_int_pos(const char *str) {
    if (!str || str[0] == '\0') 
        return false;  

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i]))
            return false;
    }

    int val = atoi(str);
    if (val <= 0)
        return false;

    return true;
}