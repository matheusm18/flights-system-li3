#include "utils/date.h"
#include <stdio.h>
#include <stdlib.h>

int string_to_date(const char* str) {
    if (!str) return 0;
    
    int year, month, day;
    int scanned = sscanf(str, "%d-%d-%d", &year, &month, &day);
    
    if (scanned != 3) return 0;
    
    return year * 10000 + month * 100 + day;
}

long string_to_datetime(const char* str) {
    if (!str) return 0;
    
    int year, month, day, hour, minute;
    int scanned = sscanf(str, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute);
    
    if (scanned != 5) return 0;
    
    return year * 100000000L + month * 1000000L + day * 10000L + hour * 100L + minute; // O 'L' força o cálculo a ser feito em long (evita overflow)
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