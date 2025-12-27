#include "validation/validate_arg.h"
#include "validation/airport_validation.h"
#include "utils/utils_validation.h"

#include <stdio.h>
#include <stdlib.h>

struct validation_result {
    bool ok;
    const char *erro;
};

ValidationResult validate_query1(char **args) {
    if (!validate_code_airport(args[0]))
        return (ValidationResult){false, "Código de aeroporto inválido (ex: LIS)"};

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query2(char **args) {
    if (!validate_int_pos(args[0]))
        return (ValidationResult){false, "O primeiro argumento deve ser um inteiro positivo"};

    return (ValidationResult){true, NULL};
}

ValidationResult validate_query3(char **args) {
    if (!validate_date(args[0]) || !validate_date(args[1]))
        return (ValidationResult){false, "Formato de data inválido (YYYY-MM-DD)"};

    return (ValidationResult){true, NULL};
}


ValidationResult* validar_query(int query, char **args) {
    ValidationResult* res = malloc(sizeof(ValidationResult));
    switch (query) {
        case 1: *res = validate_query1(args); break;
        case 2: *res = validate_query2(args); break;
        case 3: *res = validate_query3(args); break;
        default: *res = (ValidationResult){false, "Query desconhecida"}; break;
    }
    return res;
}


const char* validation_result_get_erro(ValidationResult* res) {
    return res->erro;
}

bool validation_result_get_ok(ValidationResult* res) {
    return res->ok;
}

