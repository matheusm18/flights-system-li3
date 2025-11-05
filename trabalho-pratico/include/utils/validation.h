#ifndef VALIDATION_H
#define VALIDATION_H

void init_aircrafts_errors_file();
void process_valid_line_aircrafts(char **fields, int num_fields, void* catalog);
void init_airports_errors_file();
void process_valid_line_airports(char **fields, int num_fields, void* catalog);
void init_flights_errors_file();
void process_valid_line_flights(char **fields, int num_fields, void* catalog);
void init_passengers_errors_file();
void process_valid_line_passengers(char **fields, int num_fields, void* catalog);
void init_reservations_errors_file();
void process_valid_line_reservations(char **fields, int num_fields, void* catalog);

#endif