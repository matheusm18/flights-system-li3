#ifndef FLIGHT_H
#define FLIGHT_H

#include "utils/validation_utils.h"

/**
 * @typedef Flight
 * @brief Estrutura opaca que representa um voo.
 *
 * A estrutura Flight armazena informação essencial sobre um voo, incluindo:
 *  - Identificador do voo
 *  - Datas de partida (prevista e real)
 *  - Estado do voo
 *  - Aeroportos de origem e destino
 *  - Identificador da aeronave
 *
 * A definição concreta da estrutura encontra-se no ficheiro flight.c.
 */
typedef struct flight Flight;

/**
 * @brief Cria uma nova estrutura Flight.
 *
 * Aloca e inicializa um voo com a informação fornecida.
 * * **Estratégia de Gestão de Memória:**
 * - `flight_id`, `origin`, `destination`: São COPIADOS para buffers internos de tamanho fixo.
 * - `status`, `aircraft`: São armazenados por REFERÊNCIA (ponteiros diretos).
 * * @param flight_id Identificador único do voo (Será copiado, max 8 chars).
 * @param departure Data/hora de partida prevista.
 * @param actual_departure Data/hora de partida real.
 * @param arrival Data/hora de chegada prevista (não utilizada).
 * @param actual_arrival Data/hora de chegada real (não utilizada).
 * @param gate Gate do voo (não utilizado).
 * @param status Estado do voo (NÃO duplicado; ponteiro vem da String Pool).
 * @param origin Código do aeroporto de origem (Será copiado, max 3 chars).
 * @param destination Código do aeroporto de destino (Será copiado, max 3 chars).
 * @param aircraft Identificador da aeronave (NÃO duplicado; ponteiro vem da String Pool).
 * @param airline Identificador da companhia aérea (não utilizado).
 *
 * @return Ponteiro para a estrutura Flight criada, ou NULL em caso de falha
 * na alocação de memória.
 *
 * @warning O caller deve garantir que as strings passadas em `status` e `aircraft` 
 * permanecem válidas durante o tempo de vida do objeto Flight e não são 
 * libertadas manualmente (geridas pela StringPool do CatalogManager).
 */
Flight* create_flight(const char* flight_id, long departure, long actual_departure, long arrival, long actual_arrival, const char* gate, const char* status, const char* origin, const char* destination, const char* aircraft, const char* airline);

/**
 * @brief Liberta a memória associada a um voo.
 *
 * Liberta a estrutura Flight.
 * **Nota:** NÃO liberta as strings `status` ou `aircraft` (pertencem à StringPool).
 *
 * @param f Ponteiro para o voo a destruir.
 *
 * @return void
 */
void destroy_flight(Flight* f);

/* ========================= GETTERS ========================= */

/**
 * @brief Obtém a data/hora de partida real de um voo.
 *
 * @param f Ponteiro para o voo.
 *
 * @return Data/hora de partida real.
 */
long get_flight_actual_departure(const Flight* f);

/**
 * @brief Obtém a data/hora de partida prevista de um voo.
 *
 * @param f Ponteiro para o voo.
 *
 * @return Data/hora de partida prevista.
 */
long get_flight_departure(const Flight* f);

/**
 * @brief Obtém o identificador do voo.
 *
 * @param flight Ponteiro para o voo.
 *
 * @return String dinamicamente alocada com o identificador do voo,
 *         ou NULL se flight for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_flight_id(const Flight* flight);

/**
 * @brief Obtém o aeroporto de origem do voo.
 *
 * @param f Ponteiro para o voo.
 *
 * @return String dinamicamente alocada com o código do aeroporto de origem,
 *         ou NULL se f for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_flight_origin(const Flight* f);

/**
 * @brief Obtém o aeroporto de destino do voo.
 *
 * @param f Ponteiro para o voo.
 *
 * @return String dinamicamente alocada com o código do aeroporto de destino,
 *         ou NULL se f for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_flight_destination(const Flight* f);

/**
 * @brief Obtém o estado do voo.
 *
 * @param f Ponteiro para o voo.
 *
 * @return String dinamicamente alocada com o estado do voo,
 *         ou NULL se f for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_flight_status(const Flight* f);

/**
 * @brief Obtém o identificador da aeronave associada ao voo.
 *
 * @param f Ponteiro para o voo.
 *
 * @return String dinamicamente alocada com o identificador da aeronave,
 *         ou NULL se f ou a aeronave forem NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_aircraft_id_from_flight(const Flight* f);

#endif
