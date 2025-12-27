#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>


typedef struct validation_result ValidationResult;

ValidationResult validate_query1(char **args);
ValidationResult validate_query2(char **args);
ValidationResult validate_query3(char **args);
ValidationResult* validar_query(int query, char **args);

const char* validation_result_get_erro(ValidationResult* res);
bool validation_result_get_ok(ValidationResult* res);

#endif