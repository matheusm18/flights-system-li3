#include "utils/query4_utils.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>


int is_leap(int y) {
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

int get_days_in_month_adj(int m, int y) {
    static const int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (m == 2 && is_leap(y)) return 29;
    return days_in_month[m];
}

int weekday(int y, int m, int d) {
    if (m < 3) { m += 12; y--; }
    int K = y % 100;
    int J = y / 100;
    int w = (d + 13*(m+1)/5 + K + K/4 + J/4 + 5*J) % 7;
    return ((w + 6) % 7); // 0 = dom, 1 = seg, ..., 6 = sab
}

void date_to_week_key_buf(int date, char buf[20]) {
    int year  = date / 10000;
    int month = (date / 100) % 100;
    int day   = date % 100;

    // descobrir qual o dia da semana atual (0 = dom a 6 = sab)
    int wday = weekday(year, month, day);

    // recuar até Domingo
    day = day - wday;

    // corrigir underflow de dia/mês/ano 
    while (day <= 0) {
        month--;
        if (month < 1) {
            month = 12;
            year--;
        }
        // soma os dias do mes anterior ao saldo negativo
        day += get_days_in_month_adj(month, year);
    }

    buf[0] = (year / 1000) + '0';
    buf[1] = (year / 100 % 10) + '0';
    buf[2] = (year /10 % 10) + '0';
    buf[3] = (year % 10) + '0';
    buf[4] = '-';
    buf[5] = (month / 10) + '0';
    buf[6] = (month % 10) + '0';
    buf[7] = '-';
    buf[8] = (day / 10) + '0';
    buf[9] = (day % 10) + '0';
    buf[10] = '\0';
}