#ifndef AIRPORT_H
#define AIRPORT_H

typedef struct airport Airport;

Airport* create_airport(const char *code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type);

void destroy_airport(Airport* airport);
const char* airport_type_to_string(char type);

// getters
const char* get_airport_code(const Airport* airport);
const char* get_airport_name(const Airport* airport);
const char* get_airport_city(const Airport* airport);
const char* get_airport_country(const Airport* airport);
char get_airport_type(const Airport* airport);
char get_airport_type_char(const char* type_str);

#endif
