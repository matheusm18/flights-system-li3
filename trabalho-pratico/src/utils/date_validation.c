#include "utils/date_validation.h"
#include "utils/date.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//======== Syntactic validation: Dates

bool validate_date(char* date) {
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7) {
            if(date[i] < '0' || date[i] > '9') return false;
        }
    }

    int year, month, day;
    sscanf(date,"%d-%d-%d",&year, &month, &day);

    
    return (month >= 1 && month <= 12 && day >= 1 && day <= 31);

}

bool validate_time(char* time) {
    if (strlen(time) != 5 || time[2] != ':') return false;

    for (int i = 0; i < 5; i++) {
        if (i != 2) {
            if(time[i] < '0' || time[i] > '9') return false;
        }
    }

    int hours, minutes;
    sscanf(time,"%d:%d",&hours, &minutes);

    return (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);

}

bool validate_previous_date(const char* date1, const char* date2) {
    int y1, m1, d1, h1, min1;
    int y2, m2, d2, h2, min2;

    if (sscanf(date1, "%d-%d-%d %d:%d", &y1, &m1, &d1, &h1, &min1) != 5) return false;
    if (sscanf(date2, "%d-%d-%d %d:%d", &y2, &m2, &d2, &h2, &min2) != 5) return false;

    if (y1 != y2) return y1 < y2;
    if (m1 != m2) return m1 < m2;
    if (d1 != d2) return d1 < d2;
    if (h1 != h2) return h1 < h2;
    return min1 < min2;
}

