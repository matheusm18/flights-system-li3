#ifndef AIRCRAFT_H
#define AIRCRAFT_H

typedef struct aircraft Aircraft;

Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, unsigned short year, unsigned short capacity, unsigned short range);

void destroy_aircraft(Aircraft* a);
const char* get_aircraft_identifier(const Aircraft* a);
const char* get_aircraft_manufacturer(const Aircraft* a);
const char* get_aircraft_model(const Aircraft* a);
int get_aircraft_flight_count(const Aircraft* a);
void aircraft_increment_flight_count(Aircraft* aircraft);

#endif