#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

char* int_to_string(int value);

char* double_to_string(double valor);
bool validate_date(const char* date);
bool validate_time(const char* time);
bool validate_datetime(const char* datetime);
int string_to_date(const char* str);
long string_to_datetime(const char* str);
int get_date_part(long datetime);
int compare_dates(int date1, int date2);
int compare_datetimes(long dt1, long dt2);
int get_flight_delay(long actual_departure, long expected_departure);
bool string_to_bool(const char *str);
bool validate_int_pos(const char *str);

#endif