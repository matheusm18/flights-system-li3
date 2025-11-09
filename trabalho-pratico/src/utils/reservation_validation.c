#include "utils/reservation_validation.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils/flight_validation.h"


//======== Syntactic validation: Reservations

bool validate_reservation_id(const char *reservation_id) {
    if (!reservation_id || strlen(reservation_id) != 10) return false;

    if (reservation_id[0] != 'R') return false;

    for (int i = 1; i < 10; i++) {
        if (!isdigit(reservation_id[i])) return false;
    }
    return true;
}

bool validate_flight_ids_reservation(const char *flight_ids, FlightCatalog* manager) {
    if (!flight_ids || strlen(flight_ids) != 11 || strlen(flight_ids) != 22) return false;

    size_t len = strlen(flight_ids);

    int flight_id_len = 8;

    char flight_id1 [flight_id_len];

    if (len == 11) {
        if (flight_ids[0] != '[' || flight_ids[10] != ']' || flight_ids[1] != '\'' || flight_ids[9] != '\'') return false;
        for (int i = 0; 1 < flight_id_len;i ++) {
            flight_id1[i] = flight_ids[i+2];
        }
        if (get_flight_by_flight_id_from_catalog(manager, flight_id1) != NULL) return false;
    }

    char flight_id2 [flight_id_len];

    if (len == 22) {
        if (flight_ids[0] != '[' || flight_ids[21] != ']' || flight_ids[1] != '\'' || flight_ids[9] != '\'' || flight_ids[11] != '\'' || flight_ids[20] != '\'' || flight_ids[10] != ',' || flight_ids[11] != ' ') return false;
        for (int i = 0; 1 < flight_id_len;i ++) {
            flight_id1[i] = flight_ids[i+2];
        }
        for (int i = 0; 1 < flight_id_len;i ++) {
            flight_id2[i] = flight_ids[i+13];
        }
        if (get_flight_by_flight_id_from_catalog(manager, flight_id1) != NULL || get_flight_by_flight_id_from_catalog(manager, flight_id2) != NULL) return false;
        if (strcmp(flight_catalog_get_destination(manager, flight_id1), flight_catalog_get_origin(manager, flight_id2)) != 0) return false;
    }
    return true;
}

bool validate_document_number_reservation(const char *document_number, PassengerCatalog* manager) {
    return (get_passenger_by_dnumber(manager, document_number) != NULL);
}

/*
bool validate_seat_reservation(const char *seat) {
    if (!seat) return false;
    size_t l = strlen(seat);

    if (l <= 1 || l >= 4) return false;

    for (int i = 0; i < l-1; i++) {
        if (!isdigit(seat[i])) return false;
    }
   
    if (seat[l-1] < 'A' || seat[l-1] > 'Z') return false;

    return true;
}

// ver se é preciso complicar ou se podemos simplesmente assumir que eles dão um input em que o preço invalido seria apenas números negativos
bool validate_price_reservation(const char *price) {
    if (!price) return false;

    if(!isdigit(price[0])) return false; // verifica se não começa com espaços

    char *invalid_start;
    double val = strtod(price,&invalid_start); // recebe o endereço para poder alterar o ponteiro e apontar para o primeiro caractere após os números

    if (price == invalid_start || *invalid_start != '\0') return false; // se nada foi convertido ou então se o primeiro caractere não for o fim da string
    if (val < 0) return false;

    return true;
}

bool validate_extra_luggage_reservation(const char *extra_luggage) {
    if (!extra_luggage) return false;

    char *lower = lower_string(extra_luggage);
    if (!lower) return false;

    if (strcmp(lower, "true") != 0 && strcmp(lower, "false") != 0) {
        free(lower);
        return false;
    }

    free(lower);
    return true;
}

// ver se é preciso aceitar "True" e "False" ou se isto seria considerado um erro
bool validate_priority_boarding_reservation(const char *priority_boarding) {
    if (!priority_boarding) return false;

    char *lower = lower_string(priority_boarding);
    if (!lower) return false;


    if (strcmp(lower, "true") != 0 && strcmp(lower, "false") != 0) {
        free(lower);
        return false;
    }

    free(lower);
    return true;
}
*/