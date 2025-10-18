#ifndef AIRCRAFT_VALIDATION_H
#define AIRCRAFT_VALIDATION_H

#include <stdbool.h>

bool validate_identifier_aircraft(const char *identifier);
bool validate_manufacturer_aircraft(const char *manufacturer);
bool validate_model_aircraft(const char *model);
bool validate_year_aircraft(const char *string);
bool validate_capacity_aircraft(const char *string);
bool validate_range_aircraft(const char *string);

#endif