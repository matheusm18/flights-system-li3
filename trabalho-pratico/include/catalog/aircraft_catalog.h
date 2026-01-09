#ifndef AIRCRAFT_CATALOG_H
#define AIRCRAFT_CATALOG_H

#include <glib.h>
#include "entities/aircraft.h"

/**
 * @typedef AircraftCatalog
 * @brief Estrutura opaca que representa o catálogo de aeronaves.
 *
 * O catálogo de aeronaves gere todas as aeronaves do sistema, permitindo:
 *  - Acesso rápido a aeronaves através do seu identificador
 *  - Contabilização do número de voos realizados por cada aeronave
 *  - Iteração sobre todas as aeronaves registadas
 *
 * A implementação concreta da estrutura encontra-se no ficheiro .c,
 * garantindo encapsulamento.
 */
typedef struct aircraft_catalog AircraftCatalog;

/**
 * @typedef AircraftData
 * @brief Estrutura que agrega uma aeronave e estatísticas associadas.
 *
 * Esta estrutura associa uma instância de Aircraft ao respetivo número
 * de voos realizados, sendo usada internamente pelo catálogo.
 */
typedef struct aircraft_data AircraftData;

/**
 * @typedef AircraftIter
 * @brief Iterador para percorrer todas as aeronaves do catálogo.
 *
 * Permite iterar sobre todas as entradas do catálogo de aeronaves,
 * devolvendo uma estrutura AircraftData por iteração.
 */
typedef struct aircraft_iterator AircraftIter;

/**
 * @brief Cria e inicializa um catálogo de aeronaves.
 *
 * Esta função aloca memória para um novo catálogo de aeronaves e inicializa
 * uma tabela hash que associa identificadores de aeronaves a estruturas
 * AircraftData.
 *
 * @return Ponteiro para o catálogo de aeronaves criado, ou NULL se a alocação
 *         de memória falhar.
 */
AircraftCatalog* aircraft_catalog_create();

/**
 * @brief Destrói um catálogo de aeronaves e liberta toda a memória associada.
 *
 * Esta função liberta:
 *  - Todas as aeronaves armazenadas
 *  - As estruturas AircraftData
 *  - A tabela hash interna
 *  - A própria estrutura do catálogo
 *
 * @param manager Ponteiro para o catálogo de aeronaves a destruir.
 *
 * @return void
 *
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 */
void aircraft_catalog_destroy(AircraftCatalog* manager);

/**
 * @brief Adiciona uma aeronave ao catálogo.
 *
 * Insere uma aeronave no catálogo, associando o seu identificador único
 * a uma estrutura AircraftData inicializada com contador de voos a zero.
 *
 * @param manager Ponteiro para o catálogo de aeronaves.
 * @param aircraft Ponteiro para a aeronave a adicionar.
 *
 * @return void
 *
 * @note Se manager ou aircraft forem NULL, a função não realiza nenhuma operação.
 */
void aircraft_catalog_add(AircraftCatalog* manager, Aircraft* aircraft);

/**
 * @brief Cria um iterador para percorrer o catálogo de aeronaves.
 *
 * Inicializa um iterador que permite percorrer todas as aeronaves
 * armazenadas no catálogo.
 *
 * @param catalog Ponteiro constante para o catálogo de aeronaves.
 *
 * @return Ponteiro para o iterador criado, ou NULL em caso de erro.
 */
AircraftIter* aircraft_catalog_iter_create(const AircraftCatalog* catalog);

/**
 * @brief Obtém a próxima aeronave do iterador.
 *
 * Avança o iterador e devolve a estrutura AircraftData associada
 * à aeronave corrente.
 *
 * @param it Ponteiro para o iterador.
 *
 * @return Ponteiro constante para AircraftData se existir próximo elemento,
 *         ou NULL se o iterador tiver terminado.
 */
const AircraftData* aircraft_catalog_iter_next(AircraftIter* it);

/**
 * @brief Liberta um iterador de aeronaves.
 *
 * Liberta a memória associada a um iterador previamente criado.
 *
 * @param it Ponteiro para o iterador a libertar.
 *
 * @return void
 */
void aircraft_catalog_iter_free(AircraftIter* it);

/**
 * @brief Incrementa o contador de voos de uma aeronave.
 *
 * Atualiza o número de voos realizados pela aeronave com o identificador
 * fornecido.
 *
 * @param aircraft_id Identificador da aeronave.
 * @param manager Ponteiro para o catálogo de aeronaves.
 *
 * @return void
 */
void aircrafts_counter_increment(
    const char* aircraft_id,
    AircraftCatalog* manager
);

/**
 * @brief Obtém uma aeronave através do seu identificador.
 *
 * Procura no catálogo uma aeronave associada ao identificador fornecido.
 *
 * @param manager Ponteiro para o catálogo de aeronaves.
 * @param identifier Identificador da aeronave.
 *
 * @return Ponteiro constante para a aeronave se existir,
 *         ou NULL caso contrário.
 */
const Aircraft* get_aircraft_by_identifier(
    AircraftCatalog* manager,
    const char* identifier
);

/**
 * @brief Obtém o número de voos realizados por uma aeronave.
 *
 * @param data Ponteiro constante para a estrutura AircraftData.
 *
 * @return Número de voos realizados pela aeronave, ou 0 se data for NULL.
 */
int get_aircraft_flight_count(const AircraftData* data);

/**
 * @brief Obtém o número total de aeronaves no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeronaves.
 *
 * @return Número total de aeronaves armazenadas no catálogo,
 *         ou 0 se catalog for NULL.
 */
int get_total_aircrafts_in_catalog(AircraftCatalog* catalog);

/**
 * @brief Obtém a aeronave associada a uma estrutura AircraftData.
 *
 * @param data Ponteiro constante para AircraftData.
 *
 * @return Ponteiro constante para a aeronave, ou NULL se data for NULL.
 */
const Aircraft* get_aircraft_from_data(const AircraftData* data);

#endif
