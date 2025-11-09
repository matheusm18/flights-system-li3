#ifndef RESERVATION_VALIDATION_H
#define RESERVATION_VALIDATION_H

#include "catalog/flight_catalog.h"
#include "catalog/passenger_catalog.h"
#include <stdbool.h>

bool validate_reservation_id(const char *reservation_id);
bool validate_flight_ids_reservation(const char *flight_ids, FlightCatalog* manager);
bool validate_document_number_reservation(const char *document_number, PassengerCatalog* manager);

#endif