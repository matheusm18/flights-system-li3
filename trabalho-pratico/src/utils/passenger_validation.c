#include "utils/passenger_validation.h"
#include "utils/date_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//======== Syntactic validation: Passengers

bool validate_passenger_document_number(const char *document_number){
    if (document_number == NULL) return false;
    
    // tem de ter obrigatoriamente 9 dígitos
    if (strlen(document_number) != 9){
        return false;
    }

    // todos os caracteres devem ser digitos entre 0 e 9
    for (int i = 0; i < 9; i++){
        if(!isdigit((unsigned char)document_number[i])){ // a função isdigit espera valores positivos, por isso temos de converter para unsigned
            return false;
        }
    }

    return true;
}

bool validate_passenger_gender(const char *gender) {
    if (gender == NULL) {
        return false;
    }

    return strcmp(gender, "M") == 0 || strcmp(gender, "F") == 0 || strcmp(gender, "O") == 0;
}

bool validate_passenger_email(const char *email){
    if (email == NULL){
        return false;
    }

    // na string tem de existir o caracter @ 
    const char *at = strchr(email,'@'); // procura e retorna um ponteiro para a sua primeira ocorrência
    if (!at) return false; // não encontrou

    // validar username
    for (const char* i = email; i < at; i++){
        if (!(islower((unsigned char)*i) || isdigit((unsigned char)*i) || *i == '.')){
            return false;
        }
    }

    // validar domain
    const char *domain = at + 1; // ponteiro para o inicio do domain
    const char *dot = strchr(domain, '.');  // procura o ponto no domínio
    if (!dot) return false;                  // domínio inválido 

    // valida domain antes do ponto
    if (dot == domain) return false;         // lstring vazia
    for (const char *i = domain; i < dot; i++) {
        if (!islower((unsigned char)*i)) return false;
    }

    // valida depois do ponto
    unsigned int rstring_len = strlen(dot + 1);
    if (rstring_len < 2 || rstring_len > 3) return false; 
    for (const char *i = dot + 1; *i; i++) {
        if (!islower((unsigned char)*i)) return false;
    }

    return true;

}

bool validate_passenger_birth_date(const char *dob){
    if (!validate_date(dob)) return false;
    
    return true;
}