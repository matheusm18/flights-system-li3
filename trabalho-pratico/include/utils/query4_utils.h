#ifndef QUERY4_UTILS_H
#define QUERY4_UTILS_H

#include <glib.h>


int is_leap(int y);
int get_days_in_month_adj(int m, int y);
int weekday(int y, int m, int d);
void date_to_week_key_buf(int date, char buf[20]);

#endif