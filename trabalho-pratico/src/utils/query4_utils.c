#include "utils/query4_utils.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

static const int days_before_month[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

static int is_leap(int y) {
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

// 1..365/366
static int day_of_year(int y, int m, int d) {
    int doy = days_before_month[m-1] + d;
    if (m > 2 && is_leap(y)) doy++;
    return doy;
}

// Zeller's congruence para calcular dia da semana (0=Dom,1=Seg,..6=Sáb)
static int weekday(int y, int m, int d) {
    if (m < 3) {
        m += 12;
        y--;
    }
    int K = y % 100;
    int J = y / 100;
    int w = (d + 13*(m+1)/5 + K + K/4 + J/4 + 5*J) % 7;
    return ((w + 6) % 7); // converte para 0=Dom
}

// versão rápida do cálculo da semana (domingo-sábado)
int get_week_number_fast(int year, int month, int day) {
    int doy = day_of_year(year, month, day);
    int wday_jan1 = weekday(year, 1, 1); // 1º de Janeiro

    int week = (doy + wday_jan1 - 1) / 7 + 1;
    return week;
}

// escreve diretamente no buffer fornecido
void date_to_week_key_buf(int date, char buf[20]) {
    int year  = date / 10000;
    int month = (date / 100) % 100;
    int day   = date % 100;

    int week = get_week_number_fast(year, month, day);
    snprintf(buf, 20, "%d-W%02d", year, week);
}





