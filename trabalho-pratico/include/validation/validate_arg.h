#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

/**
 * @brief Estrutura que representa o resultado de uma validação.
 * 
 * @param ok Booleano indicando se a validação passou (true) ou falhou (false)
 * @param erro Mensagem de erro (NULL se ok = true)
 */
typedef struct validation_result ValidationResult;

/**
 * @brief Valida os argumentos da Query 1 (código de aeroporto).
 * 
 * @param args Array de strings com argumentos
 * @return ValidationResult indicando sucesso ou erro
 */
ValidationResult validate_query1(char **args);

/**
 * @brief Valida os argumentos da Query 2 (top N aeronaves, fabricante opcional).
 * 
 * @param args Array de strings com argumentos
 * @return ValidationResult indicando sucesso ou erro
 */
ValidationResult validate_query2(char **args);

/**
 * @brief Valida os argumentos da Query 3 (intervalo de datas).
 * 
 * @param args Array de strings com argumentos
 * @return ValidationResult indicando sucesso ou erro
 */
ValidationResult validate_query3(char **args);

/**
 * @brief Valida os argumentos da Query 4 (intervalo de datas para ranking semanal de passageiros).
 * 
 * @param args Array de strings com argumentos
 * @return ValidationResult indicando sucesso ou erro
 */
ValidationResult validate_query4(char **args);

/**
 * @brief Valida os argumentos da Query 5 (N para top N companhias aéreas).
 * 
 * @param args Array de strings com argumentos
 * @return ValidationResult indicando sucesso ou erro
 */
ValidationResult validate_query5(char **args);

/**
 * @brief Valida os argumentos da Query 6 (nacionalidade do passageiro).
 * 
 * @param args Array de strings com argumentos
 * @return ValidationResult indicando sucesso ou erro
 */
ValidationResult validate_query6(char **args);

/**
 * @brief Valida genericamente qualquer query com base no seu número.
 * 
 * @param query Número da query (1 a 6)
 * @param args Array de strings com argumentos
 * @return Ponteiro para ValidationResult alocado dinamicamente (deve ser libertado pelo chamador)
 */
ValidationResult* validate_query(int query, char **args);

/**
 * @brief Retorna a mensagem de erro de um ValidationResult.
 * 
 * @param res Ponteiro para ValidationResult
 * @return String da mensagem de erro ou NULL se válido
 */
const char* validation_result_get_erro(ValidationResult* res);

/**
 * @brief Retorna o estado booleano de um ValidationResult.
 * 
 * @param res Ponteiro para ValidationResult
 * @return true se válido, false caso contrário
 */
bool validation_result_get_ok(ValidationResult* res);

#endif
