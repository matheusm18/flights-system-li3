#include "validation/passenger_validation.h"
#include "utils/validation_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


bool validate_passenger_document_number(const char *document_number){
    if (document_number == NULL) return false;
    
    if (strlen(document_number) != 9){
        return false;
    }

    for (int i = 0; i < 9; i++){
        if(!isdigit((unsigned char)document_number[i])){ ///< A função isdigit espera valores positivos, por isso temos de converter para unsigned
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

    const char *at = strchr(email,'@'); ///< Procura e retorna um ponteiro para a primeira ocorrência do '@'
    if (!at) return false;

    for (const char* i = email; i < at; i++){
        if (!(islower((unsigned char)*i) || isdigit((unsigned char)*i) || *i == '.')){
            return false;
        }
    }

    const char *domain = at + 1; 
    const char *dot = strchr(domain, '.');
    if (!dot) return false;                  

    if (dot == domain) return false;      
    for (const char *i = domain; i < dot; i++) {
        if (!islower((unsigned char)*i)) return false;
    }

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