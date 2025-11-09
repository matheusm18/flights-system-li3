#ifndef FLIGHT_VALIDATION_H
#define FLIGHT_VALIDATION_H

#include "catalog/aircraft_catalog.h"
#include <stdbool.h>

bool validate_flight_id_flight(const char *flight_id);

bool validate_destination_origin(const char *origin, const char *destination);
bool validate_temporal_consistency(const char *status, long departure, long arrival, long actual_departure, long actual_arrival);
bool validate_cancelled_status(const char *status, const char *actual_departure_str, const char *actual_arrival_str);
bool validate_delayed_status(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival);
bool validate_status_flight_new(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival,
                                const char *actual_departure_str, const char *actual_arrival_str);
bool validate_aircraft_flight(const char *aircraft, AircraftCatalog* aircraft_catalog);
bool validate_flight_logical(const char *origin, const char *destination, long departure, long arrival, long actual_departure, long actual_arrival,
                             const char *status, const char *aircraft, AircraftCatalog* aircraft_catalog, const char *actual_departure_str,
                             const char *actual_arrival_str);

#endif