#ifndef AIRCRAFT_H
#define AIRCRAFT_H

typedef struct aircraft Aircraft;

Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, int year, int capacity, int range);

void destroy_aircraft(Aircraft* a);
const char* get_aircraft_identifier(const Aircraft* a);
const char* get_aircraft_manufacturer(const Aircraft* a);

#endif