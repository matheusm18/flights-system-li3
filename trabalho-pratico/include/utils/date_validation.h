#ifndef DATE_VALIDATION_H
#define DATE_VALIDATION_H

#include <stdbool.h>

bool validate_date(const char* date);
bool validate_time(char* time);
bool validate_previous_date(const char* date1, const char* date2);

#endif