#include "validation/arg_validation.h"
#include "validation/airport_validation.h"
#include "utils/validation_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct validation_result {
    bool ok;
    const char *erro;
};

ValidationResult validate_query1(char **args) {
    if (!validate_code_airport(args[0]))
        return (ValidationResult){false, "Codigo de aeroporto invalido! (ex: LIS)"};

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query2(char **args) {
    if (!validate_int_pos(args[0]))
        return (ValidationResult){false, "O primeiro argumento deve ser um inteiro positivo!"};

    // se houver o segundo argumento valida-o
    if (args[1] != NULL && args[1][0] != '\0') {
        char *manufacturer = args[1];
        
        // verifica se a primeira letra é maiúscula
        if (!isupper((unsigned char)manufacturer[0])) {
            return (ValidationResult){false, "Manufacturer deve comecar com letra maiuscula!"};
        }
        
        // verifica se contém apenas letras
        for (int i = 0; manufacturer[i] != '\0'; i++) {
            if (!isalpha((unsigned char)manufacturer[i])) {
                return (ValidationResult){false, "Manufacturer deve conter apenas letras!"};
            }
        }
    }

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query3(char **args) {
    if (!validate_date(args[0]) || !validate_date(args[1]))
        return (ValidationResult){false, "Formato de data invalido! (YYYY-MM-DD)"};

    if (!validate_date(args[0]) || !validate_date(args[1])) {
        return (ValidationResult){false, "Formato de data invalido! (YYYY-MM-DD)"};
    }
    
    if (strcmp(args[0], args[1]) > 0) {
        return (ValidationResult){false, "Data de fim nao pode ser anterior a de inicio!"};
    }

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query4(char **args) {
    if (args[0] == NULL) {
        return (ValidationResult){true, NULL};
    }

    if (args[1] == NULL) {
        return (ValidationResult){false, "O segundo argumento deve existir para haver um intervalo!"};
    }

    if (!validate_date(args[0])) {
        return (ValidationResult){false, "Formato de data de inicio invalido! (YYYY-MM-DD)"};
    }

    if (!validate_date(args[1])) {
        return (ValidationResult){false, "Formato de data de fim invalido! (YYYY-MM-DD)"};
    }

    if (strcmp(args[0], args[1]) > 0) {
         return (ValidationResult){false, "Data de fim nao pode ser anterior a de inicio!"};
    }

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query5(char **args) {
    if (!validate_int_pos(args[0]))
        return (ValidationResult){false, "Argumento deve ser um inteiro positivo!"};

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query6(char **args) {    
    char *nationality = args[0];
    int len = strlen(nationality);

    // primeiro caractere deve ser letra
    if (!isalpha((unsigned char)nationality[0])) {
        return (ValidationResult){false, "Nacionalidade deve comecar com letra"};
    }
    
    // validar parênteses
    if (!validate_parentheses(nationality, len)) {
        return (ValidationResult){false, "Parenteses mal formatados"};
    }
    
    // validar caracteres permitidos
    for (int i = 0; i < len; i++) {
        char c = nationality[i];
        if (!isalnum((unsigned char)c) && c != ' ' && c != '&' && c != '(' && c != ')' && c!= '-' && c != '\'') {
            return (ValidationResult){false, "Caractere invalido na nacionalidade"};
        }
    }
    
    // validar caracteres especiais básicos
    if (!validate_basic_special_chars(nationality, len)) {
        return (ValidationResult){false, "Caracteres especiais mal posicionados"};
    }
    
    return (ValidationResult){true, NULL};
}


ValidationResult* validate_query(int query, char **args) {
    ValidationResult* res = malloc(sizeof(ValidationResult));
    switch (query) {
        case 1: *res = validate_query1(args); break;
        case 2: *res = validate_query2(args); break;
        case 3: *res = validate_query3(args); break;
        case 4: *res = validate_query4(args); break;
        case 5: *res = validate_query5(args); break;
        case 6: *res = validate_query6(args); break;
        default: *res = (ValidationResult){false, "Query desconhecida!"}; break;
    }
    return res;
}


const char* validation_result_get_erro(ValidationResult* res) {
    return res->erro;
}

bool validation_result_get_ok(ValidationResult* res) {
    return res->ok;
}

