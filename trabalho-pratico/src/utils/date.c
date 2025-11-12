#include "utils/date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int string_to_date(const char* str) {
    if (!str) return 0;
    
    int year, month, day;
    int scanned = sscanf(str, "%d-%d-%d", &year, &month, &day);
    
    if (scanned != 3) return 0;
    
    return year * 10000 + month * 100 + day;
}

long string_to_datetime(const char* str) {
    if (!str || str[0] == 'N') return -1; // "N/A"

    int y = (str[0]-'0')*1000 + (str[1]-'0')*100 + (str[2]-'0')*10 + (str[3]-'0');
    int m = (str[5]-'0')*10 + (str[6]-'0');
    int d = (str[8]-'0')*10 + (str[9]-'0');
    int h = (str[11]-'0')*10 + (str[12]-'0');
    int min = (str[14]-'0')*10 + (str[15]-'0');
    
    return y*100000000L + m*1000000L + d*10000L + h*100 + min;// O 'L' força o cálculo a ser feito em long (evita overflow)
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