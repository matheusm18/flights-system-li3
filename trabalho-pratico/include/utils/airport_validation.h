#ifndef AIRPORT_VALIDATION_H
#define AIRPORT_VALIDATION_H

#include <stdbool.h>

/**
 * @brief Valida se uma string representa um código de aeroporto válido.
 * 
* A validação consiste em verificar se o código tem
 * exatamente 3 caracteres e se todos são letras maiúsculas (A-Z).
 * 
 * @param code String contendo o código de aeroporto a ser validado.
 * 
 * @return true se o código é válido (3 caracteres maiúsculos de A-Z).
 * @return false se o código é NULL, não tem 3 caracteres, ou contém
 *         caracteres que não são letras maiúsculas.
 */
bool validate_code_airport(const char *code);

/**
 * @brief Valida se uma string representa um número decimal com no máximo 8 casas decimais.
 * 
 * Esta função verifica se a string fornecida corresponde a um número decimal válido,
 * permitindo valores negativos e garantindo que não haja mais de 8 dígitos após o
 * ponto decimal. A validação inclui verificar se há apenas um ponto decimal e se
 * todos os outros caracteres, exceto o sinal negativo inicial, são dígitos.
 * 
 * @param str String contendo o número decimal a ser validado.
 * 
 * @return true se a string representa um número decimal válido (opcionalmente negativo,
 *         com no máximo um ponto decimal e no máximo 8 casas decimais).
 * @return false se a string é NULL, está vazia após o sinal negativo, contém mais de
 *         um ponto decimal, contém caracteres não numéricos, ou tem mais de 8 casas
 *         decimais.
 */
bool validate_decimal_cases(const char *str);

/**
 * @brief Valida se uma string representa uma latitude válida de aeroporto.
 * 
 * Esta função verifica se a string fornecida corresponde a uma latitude válida,
 * que deve ser um número decimal entre -90.0 e 90.0. A validação inclui
 * verificar o formato decimal (usando validate_decimal_cases), garantir que não
 * há caracteres inválidos após a conversão, e confirmar que o valor está dentro
 * do intervalo geográfico válido para latitudes.
 * 
 * @param latitude String contendo a latitude a ser validada.
 * 
 * @return true se a latitude é válida (formato decimal correto e valor entre
 *         -90.0 e 90.0 graus, inclusive).
 * @return false se a latitude é NULL, não tem formato decimal válido, contém
 *         caracteres inválidos após o número, ou está fora do intervalo [-90.0, 90.0].
 * 
 * @see validate_decimal_cases
 */
bool validate_latitude_airport(const char *latitude);

/**
 * @brief Valida se uma string representa uma longitude válida de aeroporto.
 * 
 * Esta função verifica se a string fornecida corresponde a uma longitude válida,
 * que deve ser um número decimal entre -180.0 e 180.0 graus. A validação inclui
 * verificar o formato decimal, garantir que não há caracteres inválidos após
 * a conversão, e confirmar que o valor está dentro do intervalo geográfico
 * válido para longitudes.
 * 
 * @param longitude String contendo a longitude a ser validada.
 * 
 * @return true se a longitude é válida (formato decimal correto e valor entre
 *         -180.0 e 180.0 graus, inclusive).
 * @return false se a longitude é NULL, não tem formato decimal válido, contém
 *         caracteres inválidos após o número, ou está fora do intervalo [-180.0, 180.0].
 * 
 * @see validate_decimal_cases
 */
bool validate_longitude_airport(const char *longitude);

/**
 * @brief Valida se uma string representa um tipo de aeroporto válido.
 * 
 * Os tipos válidos são: "small_airport", "medium_airport", "large_airport", "heliport" e "seaplane_base".
 * 
 * @param type String que contêm o tipo de aeroporto a ser validado.
 * 
 * @return true se o tipo é válido (corresponde a um dos cinco tipos aceitos).
 * @return false se o tipo é NULL ou não corresponde a nenhum tipo válido.
 */
bool validate_type_airport(const char *type);

#endif