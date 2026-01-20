#ifndef AIRPORT_CATALOG_H
#define AIRPORT_CATALOG_H

#include <glib.h>
#include "entities/airport.h"
#include "entities/flight.h"

/**
 * @typedef AirportCatalog
 * @brief Estrutura opaca que representa o catálogo de aeroportos.
 *
 * O catálogo de aeroportos gere toda a informação associada aos aeroportos,
 * permitindo:
 *  - Acesso rápido a aeroportos através do código
 *  - Contabilização de chegadas e partidas
 *  - Associação de voos de partida a cada aeroporto
 *  - Iteração sobre aeroportos e respetivos voos
 *
 * A implementação concreta encontra-se no ficheiro .c, garantindo encapsulamento.
 */
typedef struct airport_catalog AirportCatalog;

/**
 * @typedef AirportData
 * @brief Estrutura que agrega um aeroporto e estatísticas associadas.
 *
 * Contém:
 *  - Ponteiro para Airport
 *  - Contadores de chegadas e partidas
 *  - Lista de voos que partiram do aeroporto
 */
typedef struct airport_data AirportData;

/**
 * @typedef AirportIter
 * @brief Iterador para percorrer todos os aeroportos do catálogo.
 *
 * Permite iterar sobre todas as entradas do catálogo de aeroportos,
 * devolvendo estruturas AirportData.
 */
typedef struct airport_iterator AirportIter;

/**
 * @typedef AirportFlightIter
 * @brief Iterador para percorrer os voos de partida de um aeroporto.
 *
 * Permite iterar sequencialmente sobre os voos associados a um aeroporto
 * específico.
 *
 * Utilizado, por exemplo, na query 3.
 */
typedef struct airport_flight_iterator AirportFlightIter;

/**
 * @brief Cria e inicializa um catálogo de aeroportos.
 *
 * Esta função aloca memória para um novo catálogo de aeroportos e inicializa
 * uma tabela hash que associa códigos de aeroporto a estruturas AirportData.
 *
 * @return Ponteiro para o catálogo de aeroportos criado, ou NULL se a alocação
 *         de memória falhar.
 */
AirportCatalog* airport_catalog_create();

/**
 * @brief Destrói um catálogo de aeroportos e liberta toda a memória associada.
 *
 * Liberta:
 *  - Todos os aeroportos armazenados
 *  - As estruturas AirportData
 *  - As listas de voos de partida
 *  - A tabela hash interna
 *  - A própria estrutura do catálogo
 *
 * @param manager Ponteiro para o catálogo de aeroportos a destruir.
 *
 * @return void
 *
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 */
void airport_catalog_destroy(AirportCatalog* manager);

/**
 * @brief Adiciona um aeroporto ao catálogo.
 *
 * Insere um aeroporto no catálogo, inicializando os contadores de chegadas
 * e partidas a zero e criando a estrutura para armazenar os voos de partida.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param airport Ponteiro para o aeroporto a adicionar.
 *
 * @return void
 */
void airport_catalog_add(AirportCatalog* manager, Airport* airport);

/**
 * @brief Adiciona um voo à lista de partidas de um aeroporto.
 *
 * Associa um voo a um aeroporto específico, registando-o como voo
 * de partida desse aeroporto.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param code Código do aeroporto.
 * @param flight Ponteiro para o voo a adicionar.
 *
 * @return void
 */
void airport_catalog_add_flight(
    AirportCatalog* manager,
    const char* code,
    Flight* flight
);

/**
 * @brief Ordena os voos de partida de todos os aeroportos.
 *
 * Ordena os voos de partida de cada aeroporto por data real de partida,
 * em ordem crescente.
 *
 * @param catalog Ponteiro para o catálogo de aeroportos.
 *
 * @return void
 */
void airport_catalog_sort_all_flights(AirportCatalog* catalog);

/**
 * @brief Incrementa o contador de passageiros de um aeroporto.
 *
 * Atualiza o número de chegadas ou partidas de um aeroporto, consoante
 * o tipo especificado.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param airport_code Código do aeroporto.
 * @param type Tipo de incremento: 'a' para chegada, 'd' para partida.
 *
 * @return void
 */
void airport_passenger_increment(
    AirportCatalog* manager,
    char* airport_code,
    char type
);

/**
 * @brief Cria um iterador para percorrer todos os aeroportos.
 *
 * @param catalog Ponteiro constante para o catálogo de aeroportos.
 *
 * @return Ponteiro para o iterador criado, ou NULL em caso de erro.
 */
AirportIter* airport_catalog_iter_create(const AirportCatalog* catalog);

/**
 * @brief Obtém o próximo aeroporto do iterador.
 *
 * Avança o iterador e devolve a estrutura AirportData associada.
 *
 * @param it Ponteiro para o iterador.
 *
 * @return Ponteiro constante para AirportData se existir próximo elemento,
 *         ou NULL se o iterador tiver terminado.
 */
const AirportData* airport_catalog_iter_next(AirportIter* it);

/**
 * @brief Liberta um iterador de aeroportos.
 *
 * @param it Ponteiro para o iterador a libertar.
 *
 * @return void
 */
void airport_catalog_iter_free(AirportIter* it);

/**
 * @brief Cria um iterador para percorrer os voos de partida de um aeroporto.
 *
 * @param data Ponteiro constante para a estrutura AirportData.
 *
 * @return Ponteiro para o iterador criado, ou NULL se data for NULL.
 */
AirportFlightIter* airport_flight_iter_create(const AirportData* data);

/**
 * @brief Obtém o próximo voo de partida do iterador.
 *
 * @param it Ponteiro para o iterador de voos.
 *
 * @return Ponteiro constante para Flight se existir próximo voo,
 *         ou NULL se o iterador tiver terminado.
 */
const Flight* airport_flight_iter_next(AirportFlightIter* it);

/**
 * @brief Liberta um iterador de voos.
 *
 * @param it Ponteiro para o iterador a libertar.
 *
 * @return void
 */
void airport_flight_iter_free(AirportFlightIter* it);

/**
 * @brief Conta o número de voos de partida num intervalo de datas.
 *
 * Conta quantos voos de um aeroporto têm data real de partida
 * compreendida entre start_date e end_date (inclusive).
 *
 * @param data Ponteiro constante para AirportData.
 * @param start_date Data inicial (YYYYMMDD).
 * @param end_date Data final (YYYYMMDD).
 *
 * @return Número de voos no intervalo especificado.
 */
int airport_catalog_count_flights_in_range(
    const AirportData* data,
    int start_date,
    int end_date
);

/**
 * @brief Obtém um aeroporto através do seu código.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param code Código do aeroporto.
 *
 * @return Ponteiro constante para Airport se existir,
 *         ou NULL caso contrário.
 */
const Airport* get_airport_by_code(
    AirportCatalog* manager,
    char* code
);

/**
 * @brief Obtém o número total de aeroportos no catálogo.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 *
 * @return Número total de aeroportos registados.
 */
int airport_catalog_get_count(AirportCatalog* manager);

/**
 * @brief Obtém o aeroporto associado a uma estrutura AirportData.
 *
 * @param data Ponteiro constante para AirportData.
 *
 * @return Ponteiro constante para Airport, ou NULL se data for NULL.
 */
const Airport* get_airport_from_data(const AirportData* data);

/**
 * @brief Obtém o número de chegadas registadas num aeroporto.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param code Código do aeroporto.
 *
 * @return Número de chegadas registadas, ou 0 se não existir.
 */
int airport_get_arrival_count(AirportCatalog* manager, char* code);

/**
 * @brief Obtém o número de partidas registadas num aeroporto.
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param code Código do aeroporto.
 *
 * @return Número de partidas registadas, ou 0 se não existir.
 */
int airport_get_departure_count(AirportCatalog* manager, char* code);

#endif
