#ifndef QUERY4_UTILS_H
#define QUERY4_UTILS_H

#include <glib.h>

typedef struct week_number WeekNumber;

WeekNumber* get_week_number(int year, int month, int day);
char* create_week_key(int year, int week);

#endif