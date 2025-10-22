#ifndef PASSENGER_VALIDATION_H
#define PASSENGER_VALIDATION_H

#include <stdbool.h>

bool validate_passenger_document_number(const char *document_number);
bool validate_passenger_gender(const char *gender);
bool validate_passenger_email(const char *email);
bool validate_passenger_birth_date(const char *dob);
bool validate_passenger_name(const char *name);

#endif