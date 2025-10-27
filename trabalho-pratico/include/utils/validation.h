#ifndef VALIDATION_H
#define VALIDATION_H

void process_valid_line_aircrafts(char **fields, int num_fields, void* catalog);
void process_valid_line_airports(char **fields, int num_fields, void* catalog);
void process_valid_line_flights(char **fields, int num_fields, void* catalog);
void process_valid_line_passengers(char **fields, int num_fields, void* catalog);
void process_valid_line_reservations(char **fields, int num_fields, void* catalog);

#endif