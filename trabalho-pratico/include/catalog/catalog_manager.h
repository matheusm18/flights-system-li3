#ifndef CATALOG_MANAGER_H
#define CATALOG_MANAGER_H

#include <glib.h>
#include "catalog/airport_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "catalog/flight_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/reservation_catalog.h"

typedef struct CatalogManager CatalogManager;

/**
 * @brief Cria e inicializa um gestor de catálogos.
 * 
 * Esta função aloca memória para um gestor de catálogos e inicializa todos
 * os catálogos individuais (aeroportos, aeronaves, voos, passageiros e reservas).
 * O gestor centraliza o acesso a todos os catálogos do sistema.
 * 
 * @return Ponteiro para o CatalogManager criado, ou NULL se a alocação
 *         de memória falhar.
 * 
 * @note O catálogo de reservas está atualmente comentado (implementação futura).
 * 
 * @see catalog_manager_destroy
 * @see airport_catalog_create
 * @see aircraft_catalog_create
 * @see flight_catalog_create
 * @see passenger_catalog_create
 */
CatalogManager* catalog_manager_create(void);


/**
 * @brief Destrói um gestor de catálogos e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para o gestor de catálogos,
 * incluindo todos os catálogos individuais e seus conteúdos. Cada catálogo
 * é destruído através da sua função de destruição específica.
 * 
 * @param manager Ponteiro para o CatalogManager a ser destruído.
 * 
 * @return void
 * 
 * @note O catálogo de reservas está atualmente comentado (implementação futura).
 *       Todos os objetos contidos nos catálogos são automaticamente destruídos.
 * 
 * @see catalog_manager_create
 * @see airport_catalog_destroy
 * @see aircraft_catalog_destroy
 * @see flight_catalog_destroy
 * @see passenger_catalog_destroy
 */
void catalog_manager_destroy(CatalogManager* manager);

/**
 * @brief Obtém o catálogo de aeroportos do gestor.
 * 
 * Esta função retorna o ponteiro para o catálogo de aeroportos gerido
 * pelo CatalogManager.
 * 
 * @param manager Ponteiro para o CatalogManager.
 * 
 * @return Ponteiro para o AirportCatalog.
 * 
 */
AirportCatalog* get_airports_from_catalog_manager(const CatalogManager* manager);

/**
 * @brief Obtém o catálogo de aeronaves do gestor.
 * 
 * Esta função retorna o ponteiro para o catálogo de aeronaves gerido
 * pelo CatalogManager.
 * 
 * @param manager Ponteiro para o CatalogManager.
 * 
 * @return Ponteiro para o AircraftCatalog.
 * 
 */
AircraftCatalog* get_aircrafts_from_catalog_manager(const CatalogManager* manager);

/**
 * @brief Obtém o catálogo de voos do gestor.
 * 
 * Esta função retorna o ponteiro para o catálogo de voos gerido
 * pelo CatalogManager.
 * 
 * @param manager Ponteiro para o CatalogManager.
 * 
 * @return Ponteiro para o FlightCatalog.
 * 
 */
FlightCatalog* get_flights_from_catalog_manager(const CatalogManager* manager);

/**
 * @brief Obtém o catálogo de passageiros do gestor.
 * 
 * Esta função retorna o ponteiro para o catálogo de passageiros gerido
 * pelo CatalogManager.
 * 
 * @param manager Ponteiro para o PassengerCatalog.
 * 
 * @return Ponteiro para o PassengerCatalog.
 * 
 */
PassengerCatalog* get_passengers_from_catalog_manager(const CatalogManager* manager);

/**
 * @brief Obtém o catálogo de reservas do gestor.
 * 
 * Esta função retorna o ponteiro para o catálogo de reservas gerido
 * pelo CatalogManager.
 * 
 * @param manager Ponteiro para o CatalogManager.
 * 
 * @return Ponteiro para o ReservationCatalog.
 * 
 * @note Esta funcionalidade está em desenvolvimento e pode retornar valores
 *       inconsistentes até a implementação completa do catálogo de reservas.
 * 
 */
ReservationCatalog* get_reservations_from_catalog_manager(const CatalogManager* manager);

/**
 * @brief Obtém um ponteiro único para uma string a partir da "String Pool" central.
 * * Esta função implementa a lógica de *String Pool* para otimização de memória.
 * Ela verifica se a string fornecida já existe na tabela de hash interna do gestor.
 * - Se existir: Retorna o ponteiro para a string já armazenada (reutilização).
 * - Se não existir: Cria uma cópia da string, armazena na pool e retorna o novo ponteiro.
 * * Esta estratégia evita a duplicação de strings repetidas (como nomes de países,
 * modelos de avião, companhias aéreas), reduzindo drasticamente o consumo de RAM.
 * * @param manager Ponteiro para o CatalogManager.
 * @param str A string original que se deseja processar.
 * * @return Ponteiro para a string única armazenada na pool.
 * * @warning O ponteiro retornado PERTENCE ao CatalogManager. **NÃO se deve executar free()**
 * neste ponteiro individualmente. A memória será libertada automaticamente
 * quando catalog_manager_destroy() for chamado.
 */
char* catalog_manager_get_string_from_pool(CatalogManager* manager, const char* str);

#endif