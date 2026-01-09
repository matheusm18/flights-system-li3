#ifndef AIRPORT_H
#define AIRPORT_H

#include "entities/flight.h"
#include <glib.h>


/**
 * @struct airport
 * @brief Estrutura que representa um aeroporto.
 *
 * A estrutura airport armazena informação essencial sobre um aeroporto, incluindo:
 *  - Código IATA do aeroporto
 *  - Nome do aeroporto
 *  - Cidade onde está localizado
 *  - País onde está localizado
 *  - Tipo de aeroporto
 */
typedef struct airport Airport;

/**
 * @brief Cria um novo objeto Airport.
 * * Aloca memória para a estrutura Airport.
 * - O código IATA (`code`) é COPIADO internamente (max 3 chars).
 * - Os campos `name`, `city` e `country` são armazenados por REFERÊNCIA.
 * * @param code String com o código IATA (Será copiado).
 * @param name Ponteiro para o nome (NÃO é duplicado; vem da String Pool).
 * @param city Ponteiro para a cidade (NÃO é duplicado; vem da String Pool).
 * @param country Ponteiro para o país (NÃO é duplicado; vem da String Pool).
 * @param latitude Latitude (não utilizada).
 * @param longitude Longitude (não utilizada).
 * @param icao Código ICAO (não utilizado).
 * @param type Tipo de aeroporto.
 * * @return Ponteiro para o objeto Airport criado, ou NULL se falhar.
 * * @warning O caller não deve libertar `name`, `city` ou `country` visto que estes pertencem à String Pool.
 */
Airport* create_airport(const char *code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type);

/**
 * @brief Destrói um objeto Airport.
 * * Liberta a estrutura Airport e o array de voos.
 * NÃO liberta as strings `name`, `city` ou `country` (pertencem à StringPool).
 * * @param airport Ponteiro para o objeto Airport a ser destruído.
 */
void destroy_airport(Airport* airport);

/**
 * @brief Obtém o código IATA de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm o código, ou NULL se airport for NULL.
 */
char* get_airport_code(const Airport* airport);

/**
 * @brief Obtém o nome de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm o nome do aeroporto, ou NULL se airport for NULL.
 */
char* get_airport_name(const Airport* airport);

/**
 * @brief Obtém a cidade de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm a cidade, ou NULL se airport for NULL.
 */
char* get_airport_city(const Airport* airport);

/**
 * @brief Obtém o país de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm o país, ou NULL se airport for NULL.
 */
char* get_airport_country(const Airport* airport);

/**
 * @brief Obtém a representação em string do tipo de um aeroporto.
 * * Esta função acede ao campo de tipo da estrutura Airport ('S', 'M', 'L')
 * e retorna a string descritiva correspondente.
 * * @note A string retornada é alocada dinamicamente (strdup). 
 * O caller é responsável por fazer free() da memória.
 * * @param airport Ponteiro para a entidade Airport (não deve ser NULL).
 * * @return String contendo o tipo ("small_airport", "medium_airport", "large_airport") ou "unknown".
 */
char* get_airport_type(const Airport* airport);

/**
 * @brief Converte uma string de tipo de aeroporto para código interno (char).
 * 
 * Esta função converte a representação em string do tipo de aeroporto
 * para o código interno de um único caractere usado no sistema.
 * 
 * @param type_str String que contêm o tipo ("small_airport", "medium_airport", "large_airport").
 * 
 * @return Caractere que contêm o tipo ('S', 'M', 'L'), ou 'U' (unknown) se não reconhecido.
 * 
 */
char airport_type_to_char(const char* type_str);

#endif
