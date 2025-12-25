#ifndef PASSENGER_H
#define PASSENGER_H

#include "utils/utils_validation.h"

typedef struct passenger Passenger;

Passenger* create_passenger(char* document_number, char* first_name, char* last_name, int dob, char* nationality, char* gender);

void destroy_passenger(Passenger* p);

// getters

int get_passenger_dnumber(const Passenger* passenger);

#endif