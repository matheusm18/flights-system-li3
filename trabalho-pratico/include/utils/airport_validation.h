#ifndef AIRPORT_VALIDATION_H
#define AIRPORT_VALIDATION_H

#include <stdbool.h>

bool validate_code_airport(const char *code);
bool validate_name_city_airport(const char *name);
bool validate_country_airport(const char *country);
bool validate_decimal_cases(const char *str);
bool validate_latitude_airport(const char *latitude);
bool validate_longitude_airport(const char *longitude);
bool validate_icao_airport(const char *icao);
bool validate_type_airport(const char *type);


#endif