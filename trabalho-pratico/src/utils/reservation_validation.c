#include "utils/reservation_validation.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//======== Syntactic validation: Reservations

bool validate_reservation_id(const char *reservation_id) {
    if (!reservation_id || strlen(reservation_id) != 10) return false;

    if (reservation_id[0] != 'R') return false;

    for (int i = 1; i < 10; i++) {
        if (!isdigit(reservation_id[i])) return false;
    }
    return true;
}

// lógica ainda não está terminada, ver como fazer e delimitar
bool validate_flight_ids_reservation(const char *flight_ids) {
    if (!flight_ids) return false;

    size_t len = strlen(flight_ids);

    if (len < 2 || flight_ids[0] != '[' || flight_ids[len-1] != ']') return false;

    return true;
}

bool validate_document_number_reservation(const char *document_number) {
    if (!document_number|| strlen(document_number) != 9) return false;

    for (int i = 0; i < 9; i++) {
        if (!isdigit(document_number[i])) return false;
    }
    return true;
}

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

    char *endptr;
    double val = strtod(price,&endptr); // recebe o endereço para poder alterar o ponteiro e apontar para o primeiro caractere após os números

    if (price == endptr || *endptr != '\0') return false; // se nada foi convertido ou então se o primeiro caractere não for o fim da string
    if (val < 0) return false;

    return true;
}

// ver se é preciso aceitar "True" e "False" ou se isto seria considerado um erro
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

// chars usados em qr code
bool is_base64_char(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           c == '+' || c == '/' || c == '=';
}

bool validate_qr_code_reservation(const char *qr_code) {
    if (!qr_code) return false;

    size_t len = strlen(qr_code);

    if (len < 5 || strncmp(qr_code, "iVBOR", 5) != 0) return false;

    for (size_t i = 0; i < len; i++) {
        if (!is_base64_char(qr_code[i])) return false;
    }

    return true;
}