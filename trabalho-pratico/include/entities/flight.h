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
 * Alguns parâmetros existem apenas para compatibilidade com o parser
 * e não são armazenados internamente.
 *
 * @param flight_id Identificador único do voo.
 * @param departure Data/hora de partida prevista.
 * @param actual_departure Data/hora de partida real.
 * @param arrival Data/hora de chegada prevista (não utilizada).
 * @param actual_arrival Data/hora de chegada real (não utilizada).
 * @param gate Gate do voo (não utilizado).
 * @param status Estado do voo.
 * @param origin Código do aeroporto de origem.
 * @param destination Código do aeroporto de destino.
 * @param aircraft Identificador da aeronave.
 * @param airline Identificador da companhia aérea (não utilizado).
 *
 * @return Ponteiro para a estrutura Flight criada, ou NULL em caso de falha
 *         na alocação de memória.
 *
 * @note As strings são copiadas internamente.
 */
Flight* create_flight(
    char* flight_id,
    long departure,
    long actual_departure,
    long arrival,
    long actual_arrival,
    const char* gate,
    const char* status,
    const char* origin,
    const char* destination,
    const char* aircraft,
    const char* airline
);

/**
 * @brief Liberta a memória associada a um voo.
 *
 * Liberta todas as strings internas e a própria estrutura Flight.
 *
 * @param f Ponteiro para o voo a destruir.
 *
 * @return void
 *
 * @note Se f for NULL, a função não realiza nenhuma operação.
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
