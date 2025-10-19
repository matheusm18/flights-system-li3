#ifndef RESERVATION_VALIDATION_H
#define RESERVATION_VALIDATION_H

#include <stdbool.h>

bool validate_reservation_id(const char *reservation_id);
bool validate_flight_ids_reservation(const char *flight_ids);
bool validate_document_number_reservation(const char *document_number);
bool validate_seat_reservation(const char *seat);
bool validate_price_reservation(const char *price);
bool validate_extra_luggage_reservation(const char *extra_luggage);
bool validate_priority_boarding_reservation(const char *priority_boarding);
bool is_base64_char(char c);
bool validate_qr_code_reservation(const char *qr_code);

#endif