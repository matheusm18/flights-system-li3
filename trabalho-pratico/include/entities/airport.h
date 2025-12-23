#ifndef AIRPORT_H
#define AIRPORT_H

#include "entities/flight.h"
#include <glib.h>

typedef struct airport Airport;

/**
 * @brief Cria um novo objeto Airport.
 * 
 * Esta função aloca memória e inicializa um novo objeto Airport com os
 * dados fornecidos. O código IATA é copiado com limite de 3 caracteres,
 * enquanto name, city e country são duplicados dinamicamente. Inicializa
 * também um array dinâmico (GPtrArray) para armazenar voos de partida.
 * 
 * @param code String que contêm o código IATA do aeroporto (3 caracteres).
 * @param name String que contêm o nome do aeroporto.
 * @param city String que contêm a cidade do aeroporto.
 * @param country String que contêm o país do aeroporto.
 * @param latitude String que contêm a latitude (atualmente não utilizada).
 * @param longitude String que contêm a longitude (atualmente não utilizada).
 * @param icao String que contêm o código ICAO (atualmente não utilizado).
 * @param type String que contêm o tipo de aeroporto.
 * 
 * @return Ponteiro para o objeto Airport criado, ou NULL se a alocação falhar.
 * 
 * @note Os campos latitude, longitude e icao estão atualmente comentados
 *       (implementação futura). O código é limitado a 3 caracteres mais
 *       o terminador '\0'.
 * 
 */
Airport* create_airport(const char *code, const char* name, const char* city, const char* country, const char* latitude, const char* longitude, const char* icao, const char* type);

/**
 * @brief Destrói um objeto Airport e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para um objeto Airport,
 * incluindo as strings alocadas dinamicamente (name, city, country),
 * o array de voos de partida, e a própria estrutura.
 * 
 * @param a Ponteiro para o objeto Airport a ser destruído.
 * 
 * @return void
 * 
 * @note Se a for NULL, a função não realiza nenhuma operação.
 *       O array de voos é libertado mas os objetos Flight em si não são
 *       destruídos (são geridos pelo FlightCatalog).
 * 
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
 * O chamador é responsável por fazer free() da memória.
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
