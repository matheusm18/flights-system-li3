#include "utils/flight_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils/date_validation.h"
#include "utils/airport_validation.h"
#include "utils/aircraft_validation.h"

//======== Syntactic validation: Flights

bool validate_flight_id_flight(const char *flight_id) {
    if (!flight_id || strlen(flight_id) != 7) return false;
    for (int i = 0; i < 2; i++) {
        if (flight_id[i] < 'A' || flight_id[i] > 'Z') return false;
    }
    for (int i = 2; i < 7; i++) {
        if (flight_id[i] < '0' || flight_id[i] > '9') return false;
    }
    return true;
}

bool validate_arrivals_and_departures_flight(const char *datetime) {
    if (!datetime || strlen(datetime) != 16) return false;
    char date[11]; // "YYYY-MM-DD" + '\0'
    char time[6];  // "HH:MM" + '\0'
    if (sscanf(datetime, "%10s %5s", date, time) != 2) return false;
    return (validate_date(date) && validate_time(time));
}

bool validate_actual_arrivals_and_departures_flight(const char *datetime1, const char *datetime2) {
    if (strcmp(datetime2, "N/A") == 0) return true;
    if (!validate_arrivals_and_departures_flight(datetime2)) return false;
    if (strcmp(datetime1, datetime2) == 0) return true;
    return validate_previous_date(datetime1, datetime2);
}


bool validate_gate_flight(const char *gate) {
    if (!gate || (strlen(gate) != 1 && strlen(gate) != 2)) return false;
    for (int i = 0; gate[i]; i++) {
        if (gate[i] < '0' || gate[i] > '9') return false;
    }
    return true;
}

bool validate_status_flight(const char *status, const char *departure, const char *arrival, const char *act_departure, const char *act_arrival) {
    if (!status) return false;

    return ((strcmp(status, "On Time") && strcmp(departure, act_departure) == 0 && strcmp(arrival, act_arrival)) == 0 ||
            (strcmp(status, "Delayed") == 0 && validate_previous_date(departure, act_departure) && validate_previous_date(arrival, act_arrival)) ||
            (strcmp(status, "Canceled") == 0 && strcmp(act_departure, "N/A") == 0 && strcmp(act_arrival, "N/A")) == 0);
}

bool validate_origin_flight(const char *origin) {
    return validate_code_airport(origin);
}

bool validate_destination_flight(const char *origin, const char *destination) {
    validate_code_airport(destination);
    if (strcmp(origin, destination) == 0) return false;
    return true;
}

bool validate_aircraft_flight(const char *aircraft) {
    return validate_identifier_aircraft(aircraft);
}

bool validate_airline_flight(const char *airline) {
    if (!airline) return false;
    for (int i = 0; airline[i]; i++) {
        unsigned char c = (unsigned char) airline[i];
        if (c < 32) return false;
    }
    return true;
}


bool validate_tracking_url_flight(const char *url, const char *flight_id) {
    if (!url) return false;
    if (strlen(url) != 37 + 7) return false;
    if (strncmp(url, "https://www.flighttracker.com/flight/", 37) != 0) return false;
    if (strncmp(url + 37, flight_id, 7) != 0) return false;
    return true;
}

