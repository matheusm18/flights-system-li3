#ifndef DATE_VALIDATION_H
#define DATE_VALIDATION_H

#include <stdbool.h>

bool validate_date(const char* date);
bool validate_time(const char* time);
bool validate_datetime(const char* datetime);
bool is_date_in_range(int date, int start_date, int end_date);

#endif