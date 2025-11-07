#ifndef FLIGHT_VALIDATION_H
#define FLIGHT_VALIDATION_H

#include <stdbool.h>

bool validate_flight_id_flight(const char *flight_id);
bool validate_arrivals_and_departures_flight(const char *datetime);
bool validate_actual_arrivals_and_departures_flight(const char *datetime1, const char *datetime2);
bool validate_status_flight(const char *status, const char *departure, const char *arrival, const char *act_departure, const char *act_arrival);
bool validate_origin_flight(const char *origin);
bool validate_destination_flight(const char *origin, const char *destination);
bool validate_aircraft_flight(const char *aircraft);

#endif