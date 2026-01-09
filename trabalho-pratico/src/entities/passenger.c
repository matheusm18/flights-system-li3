#include "entities/passenger.h"
#include <stdlib.h>
#include <string.h>

struct passenger {
    int document_number;
    int dob;
    const char* first_name;
    const char* last_name;
    const char* nationality;
    // char gender;
    // email, phone, address, photo não são armazenados

};

Passenger* create_passenger(const char* document_number, const char* first_name, const char* last_name, int dob, const char* nationality) {

    Passenger* passenger = malloc(sizeof(Passenger));
    if (passenger == NULL) return NULL;

    passenger->document_number = atoi(document_number);
    passenger->dob = dob;
    passenger->first_name = first_name;
    passenger->last_name = last_name;
    passenger->nationality = nationality;

    return passenger;
}

void destroy_passenger(Passenger* p) {
    if (p != NULL) {

        free(p);
    }
}

int get_passenger_dnumber(const Passenger* passenger) {
    if (passenger == NULL) return -1;
    
    return passenger->document_number;
}

int get_passenger_dob(const Passenger* passenger) {
    if (passenger == NULL) return -1;
    
    return passenger->dob;
}

char* get_passenger_first_name(const Passenger* passenger) {
    if (passenger == NULL) return NULL;
    
    return strdup(passenger->first_name);
}

char* get_passenger_last_name(const Passenger* passenger) {
    if (passenger == NULL) return NULL;
    
    return strdup(passenger->last_name);
}

char* get_passenger_nationality(const Passenger* passenger) {
    if (passenger == NULL) return NULL;
    
    return strdup(passenger->nationality);
}