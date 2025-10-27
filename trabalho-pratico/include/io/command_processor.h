#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "catalog/airport_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "catalog/flight_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/reservation_catalog.h"

void process_commands(const char* commands_file, AirportCatalog* airport_catalog);

#endif