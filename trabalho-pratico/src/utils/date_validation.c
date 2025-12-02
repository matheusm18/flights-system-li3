#include "utils/date_validation.h"
#include "utils/date.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//======== Validation: Dates

bool validate_date(const char* date) {
    if (!date) return false;
    
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7) {
            if(date[i] < '0' || date[i] > '9') return false;
        }
    }

    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return false;

    if (month < 1 || month > 12 || day < 1 || day > 31) return false;

    int input_date = year * 10000 + month * 100 + day;
    int current_date = 2025 * 10000 + 9 * 100 + 30;
    
    return input_date <= current_date;
}

bool validate_time(const char* time) {
    if (!time) return false;
    
    if (strlen(time) != 5 || time[2] != ':') return false;

    
    for (int i = 0; i < 5; i++) {
        if (i != 2) { ///< // Verifica se todos os caracteres, exceto ':' são digitos
            if (time[i] < '0' || time[i] > '9') return false;
        }
    }

    int hours, minutes;
    if (sscanf(time, "%d:%d", &hours, &minutes) != 2) return false;

    return (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);
}


bool validate_datetime(const char* datetime) {
    if (!datetime) return false;

    if (strcmp(datetime, "N/A") == 0) return true; ///< Aceitar 'N/A' como válido
    
    if (strlen(datetime) < 16) return false;

    char date_part[11], time_part[6];
    if (sscanf(datetime, "%10s %5s", date_part, time_part) != 2)
        return false;

    return validate_date(date_part) && validate_time(time_part);
}

