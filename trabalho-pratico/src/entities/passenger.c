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
    char* email; // será necessário armazenar?
    char* phone;
    char* address;
    char* photo; // será necessário armazenar?

};

Passenger* create_passenger(const char* document_number, const char* first_name, const char* last_name, Date* dob, const char* nationality, const char* gender, const char* email,
                            const char* phone, const char* address, const char* photo) {

    Passenger* passenger = malloc(sizeof(Passenger));
    if (passenger == NULL) return NULL;

    passenger->document_number = strdup(document_number);
    passenger->first_name = strdup(first_name);
    passenger->last_name = strdup(last_name);
    passenger->dob = date_create(date_get_year(dob),date_get_month(dob), date_get_day(dob));
    passenger->nationality = strdup(nationality);
    passenger->gender = strdup(gender);
    passenger->email = strdup(email);
    passenger->phone = strdup(phone);
    passenger->address = strdup(address);
    passenger->photo = strdup(photo);

    return passenger;
}

void destroy_passenger(Passenger* p) {
    if (p != NULL) {

        free(p->document_number);
        free(p->first_name);
        free(p->last_name);
        date_destroy(p->dob);
        free(p->nationality);
        free(p->gender);
        free(p->email);
        free(p->phone);
        free(p->address);
        free(p->photo);

        free(p);
    }
}
