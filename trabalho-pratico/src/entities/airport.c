#include "entities/airport.h"
#include <stdlib.h>
#include <string.h>

struct airport {
    char code[4];
    char* name;
    char* city;
    char* country;
    //double latitude;
    //double longitude;
    //char icao[5];
    char type;

    GPtrArray* departing_flights;
};

Airport* create_airport(const char *code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type) {
    (void) latitude;
    (void) longitude;
    (void) icao;
    
    Airport* airport = malloc(sizeof(Airport));
    if (airport == NULL) return NULL;
    
    strncpy(airport->code, code, 3);
    airport->code[3] = '\0';
    airport->name = strdup(name);
    airport->city = strdup(city);
    airport->country = strdup(country);
    //airport->latitude = atof(latitude);
    //airport->longitude = atof(longitude);
    //strncpy(airport->icao, icao, 5);
    airport->type = airport_type_to_char(type);
    
    airport->departing_flights = g_ptr_array_new();

    return airport;
}

void destroy_airport(Airport* a) {
    if (a != NULL) {

        free(a->name);
        free(a->city);
        free(a->country);

        if (a->departing_flights != NULL) {
            g_ptr_array_free(a->departing_flights, TRUE); 
        }

        free(a);
    }
}

void airport_add_departing_flight(const Airport* airport, Flight* flight) {
    if (!airport || !flight) return;
    g_ptr_array_add(airport->departing_flights, flight);
}


int compare_flight_actual_departure(const void* a, const void* b) {
    Flight* f1 = *(Flight**)a;
    Flight* f2 = *(Flight**)b;

    long dt1 = get_flight_actual_departure(f1);
    long dt2 = get_flight_actual_departure(f2);

    if (dt1 <= 0 && dt2 <= 0) return 0;
    if (dt1 <= 0) return 1;   
    if (dt2 <= 0) return -1; 

    if (dt1 < dt2) return -1;
    if (dt1 > dt2) return 1;
    return 0;
}


void airport_departing_iter_init(const Airport* airport, guint* index) {
    if (!airport || !index) return;
    *index = 0;
}


const Flight* airport_departing_iter_next(const Airport* airport, guint* index) {
    if (!airport || !index) return NULL;

    GPtrArray* flights = airport->departing_flights; 
    if (!flights) return NULL;

    if (*index >= flights->len) return NULL;  

    return g_ptr_array_index(flights, (*index)++);
}

void airport_sort_departing_flights(Airport* airport) {
    if (!airport || !airport->departing_flights) return;
    g_ptr_array_sort(airport->departing_flights, compare_flight_actual_departure);
}

char* get_airport_code(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->code);
}

char* get_airport_name(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->name);
}

char* get_airport_city(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->city);
}

char* get_airport_country(const Airport* airport) {
    if (airport == NULL) return NULL;
    return strdup(airport->country);
}

char* get_airport_type(const Airport* airport) {
    if (airport->type == 'S') return strdup("small_airport");
    else if (airport->type == 'M') return strdup("medium_airport");
    else if (airport->type == 'L') return strdup("large_airport");
    else return strdup("unknown");
}

char airport_type_to_char(const char* type_str) {
    if (strcmp(type_str, "small_airport") == 0) return 'S';
    else if (strcmp(type_str, "medium_airport") == 0) return 'M';
    else if (strcmp(type_str, "large_airport") == 0) return 'L';
    else return 'U'; // unknown
}
