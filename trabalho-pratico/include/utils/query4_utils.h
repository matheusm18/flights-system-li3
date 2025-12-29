#ifndef QUERY4_UTILS_H
#define QUERY4_UTILS_H

#include <glib.h>

int get_week_number_fast(int year, int month, int day);
void date_to_week_key_buf(int date, char buf[20]) ;

#endif