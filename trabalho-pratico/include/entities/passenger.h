#ifndef PASSENGER_H
#define PASSENGER_H

#include "utils/date.h"

typedef struct passenger Passenger;

Passenger* create_passenger(const char* document_number, const char* first_name, const char* last_name, Date* dob, const char* nationality, const char* gender, const char* email,
                            const char* phone, const char* address, const char* photo);

void destroy_passenger(Passenger* p);
const char* get_passenger_dnumber(const Passenger* passenger);

#endif