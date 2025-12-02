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
 * @brief Converte um código de tipo de aeroporto (char) para string.
 * 
 * Esta função converte o código interno de tipo de aeroporto (S, M, L)
 * para sua representação em string.
 * 
 * @param type Caractere representando o tipo ('S', 'M', 'L').
 * 
 * @return String contendo o tipo completo ("small_airport", "medium_airport",
 *         "large_airport"), ou "unknown" se o tipo não for reconhecido.
 * 
 */
const char* airport_type_to_string(char type);

/**
 * @brief Adiciona um voo à lista de partidas do aeroporto.
 * 
 * Esta função adiciona um ponteiro para um objeto Flight ao array de voos
 * de partida do aeroporto. O voo não é copiado, apenas a referência é
 * armazenada.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @param flight Ponteiro para o objeto Flight a ser adicionado.
 * 
 * @return void
 * 
 * @note Se airport ou flight forem NULL, a função não realiza nenhuma operação.
 * 
 */
void airport_add_departing_flight(const Airport* airport, Flight* flight);

/**
 * @brief Função de comparação para ordenar voos por data de partida real.
 * 
 * Esta função compara dois voos com base na sua data e hora de partida real
 * (actual_departure). Voos com partida inválida (-1 ou "N/A") são colocados
 * no final. É usada como função de comparação para g_ptr_array_sort().
 * 
 * @param a Ponteiro para o primeiro Flight* (como void**).
 * @param b Ponteiro para o segundo Flight* (como void**).
 * 
 * @return Valor negativo se a < b, 0 se a == b, valor positivo se a > b.
 * 
 * @note Voos com actual_departure inválido (≤ 0) são considerados maiores
 *       que todos os outros.
 * 
 */
int compare_flight_actual_departure(const void* a, const void* b);

/**
 * @brief Inicializa um iterador para percorrer os voos de partida do aeroporto.
 * 
 * Esta função inicializa um índice para iterar sobre o array de voos de
 * partida de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @param index Ponteiro para a variável de índice a ser inicializada (definida como 0).
 * 
 * @return void
 * 
 * @note Se airport ou index forem NULL, a função não realiza nenhuma operação.
 * 
 */
void airport_departing_iter_init(const Airport* airport, guint* index);

/**
 * @brief Obtém o próximo voo de partida do iterador.
 * 
 * Esta função retorna o próximo voo do array de partidas e incrementa o
 * índice. 
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @param index Ponteiro para a variável de índice (será incrementada).
 * 
 * @return Ponteiro constante para o próximo Flight, ou NULL se não houver
 *         mais voos ou se os parâmetros forem inválidos.
 * 
 */
const Flight* airport_departing_iter_next(const Airport* airport, guint* index);


/**
 * @brief Ordena os voos de partida do aeroporto por data de partida real.
 * 
 * Esta função ordena o array de voos de partida do aeroporto usando a
 * função compare_flight_actual_departure(). 
 * 
 * @param airport Ponteiro para o objeto Airport.
 * 
 * @return void
 * 
 * @note Se airport ou o array de voos forem NULL, a função não realiza
 *       nenhuma operação.
 * 
 * @see compare_flight_actual_departure
 * @see airport_add_departing_flight
 */
void airport_sort_departing_flights(Airport* airport);

/**
 * @brief Obtém o código IATA de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm o código, ou NULL se airport for NULL.
 */
const char* get_airport_code(const Airport* airport);

/**
 * @brief Obtém o nome de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm o nome do aeroporto, ou NULL se airport for NULL.
 */
const char* get_airport_name(const Airport* airport);

/**
 * @brief Obtém a cidade de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm a cidade, ou NULL se airport for NULL.
 */
const char* get_airport_city(const Airport* airport);

/**
 * @brief Obtém o país de um aeroporto.
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return String que contêm o país, ou NULL se airport for NULL.
 */
const char* get_airport_country(const Airport* airport);

/**
 * @brief Obtém o tipo de um aeroporto (como caractere).
 * 
 * @param airport Ponteiro para o objeto Airport.
 * @return Caractere que representa o tipo ('S', 'M', 'L'), ou '\0' se airport for NULL.
 * 
 */
char get_airport_type(const Airport* airport);

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
char get_airport_type_char(const char* type_str);

#endif
