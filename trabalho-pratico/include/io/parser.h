#ifndef PARSER_H
#define PARSER_H

#include "catalog/airport_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "catalog/flight_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/reservation_catalog.h"

void remove_quotation_marks(char *s);
void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields, void* catalog), void* catalog);
void load_datasets(const char* dataset_path, AirportCatalog* airport_catalog, AircraftCatalog* aircraft_catalog, FlightCatalog* flight_catalog, PassengerCatalog* passenger_catalog, ReservationCatalog* reservation_catalog);

#endif