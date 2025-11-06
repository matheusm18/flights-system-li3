#include "entities/passenger.h"
#include <stdlib.h>
#include <string.h>

struct passenger {
    char* document_number;
    char* first_name;
    char* last_name;
    Date* dob;
    char* nationality;
    char* gender;
    // email, phone, address, photo não são armazenados para já

};

Passenger* create_passenger(const char* document_number, const char* first_name, const char* last_name, Date* dob, const char* nationality, const char* gender) {

    Passenger* passenger = malloc(sizeof(Passenger));
    if (passenger == NULL) return NULL;

    passenger->document_number = strdup(document_number);
    passenger->first_name = strdup(first_name);
    passenger->last_name = strdup(last_name);
    passenger->dob = dob;
    passenger->nationality = strdup(nationality);
    passenger->gender = strdup(gender);

    return passenger;
}

void destroy_passenger(Passenger* p) {
    if (p != NULL) {

        free(p->document_number);
        free(p->first_name);
        free(p->last_name);
        free(p->nationality);
        free(p->gender);

        date_destroy(p->dob);

        free(p);
    }
}

const char* get_passenger_dnumber(const Passenger* passenger) {
    if (passenger == NULL) return NULL;
    return passenger->document_number;
}
