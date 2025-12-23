#ifndef AIRPORT_CATALOG_H
#define AIRPORT_CATALOG_H

#include <glib.h>
#include "entities/airport.h"

typedef struct airport_iterator AirportIter;
typedef struct airport_flight_iterator AirportFlightIter;
typedef struct airport_data AirportData;
typedef struct airport_catalog AirportCatalog;

/**
 * @brief Cria e inicializa um catálogo de aeroportos.
 * 
 * Esta função aloca memória para um novo catálogo de aeroportos e inicializa
 * uma tabela hash (GHashTable) para armazenar os aeroportos indexados pelo
 * seu código IATA. A tabela hash é configurada para liberar automaticamente
 * a memória das chaves (strings) e dos valores (objetos Airport) quando
 * os elementos são removidos ou a tabela é destruída.
 * 
 * @return Ponteiro para o catálogo de aeroportos criado, ou NULL se a alocação
 *         de memória falhar.
 * 
 * @note A tabela hash utiliza g_str_hash para função de hash e g_str_equal
 *       para comparação de chaves. As chaves são libertadas com g_free e os
 *       valores com destroy_airport.
 * 
 * @see destroy_airport
 */
AirportCatalog* airport_catalog_create(void);

/**
 * @brief Destrói um catálogo de aeroportos e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para o catálogo de aeroportos,
 * incluindo a tabela hash e todos os objetos Airport nela contidos. A função
 * g_hash_table_destroy invoca automaticamente as funções de destruição
 * configuradas (g_free para as chaves e destroy_airport para os valores).
 * 
 * @param manager Ponteiro para o catálogo de aeroportos a ser destruído.
 * 
 * @return void
 * 
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 *       Todos os objetos Airport no catálogo são automaticamente destruídos
 *       devido à configuração da tabela hash com destroy_airport.
 * 
 */
void airport_catalog_destroy(AirportCatalog* manager);

/**
 * @brief Adiciona um aeroporto ao catálogo.
 * 
 * Esta função adiciona um objeto Airport ao catálogo, indexando-o pelo seu
 * código. O código é duplicado (g_strdup) para servir como chave na
 * tabela hash. Se já existir um aeroporto com o mesmo código, ele será
 * substituído e o anterior será automaticamente destruído.
 * 
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param airport Ponteiro para o objeto Airport a ser adicionado.
 * 
 * @return void
 * 
 * @note Se manager, airport ou o código do aeroporto forem NULL, a função
 *       não realiza nenhuma operação. O código é duplicado para garantir
 *       que a tabela hash tenha sua própria cópia da chave.
 * 
 * @see get_airport_code
 * @see airport_catalog_create
 */
void airport_catalog_add(AirportCatalog* manager, Airport* airport);

void airport_catalog_add_flight(AirportCatalog* manager, const char* code, Flight* flight);

int compare_flight_actual_departure(const void* a, const void* b);

/**
 * @brief Ordena as listas de voos de partida de todos os aeroportos no catálogo.
 * 
 * Esta função percorre todos os aeroportos no catálogo e ordena as suas
 * listas de voos de partida. É chamada após o carregamento
 * de todos os dados para preparar os aeroportos para consultas.
 * 
 * @param catalog Ponteiro para o catálogo de aeroportos.
 * 
 * @return void
 * 
 * @note Se catalog for NULL, a função não realiza nenhuma operação.
 * 
 * @see airport_sort_departing_flights
 */
void airport_catalog_sort_all_flights(AirportCatalog* catalog);


void airport_passenger_increment(AirportCatalog* manager, char* airport_code, char type);

// iterar aeroportos

AirportIter* airport_catalog_iter_create(const AirportCatalog* catalog);

const AirportData* airport_catalog_iter_next(AirportIter* it);

void airport_catalog_iter_free(AirportIter* it);


// iterar voos de um aeroporto

AirportFlightIter* airport_flight_iter_create(const AirportData* data);

const Flight* airport_flight_iter_next(AirportFlightIter* it);

void airport_flight_iter_free(AirportFlightIter* it);


/**
 * @brief Obtém um aeroporto do catálogo pelo seu código.
 * 
 * Esta função procura e retorna um aeroporto no catálogo usando o seu
 * código como chave de pesquisa. A pesquisa é realizada na tabela
 * hash do catálogo, garantindo acesso eficiente em tempo O(1) médio.
 * 
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param code String que contêm o código IATA do aeroporto a procurar.
 * 
 * @return Ponteiro constante para o objeto Airport se encontrado, ou NULL
 *         se o aeroporto não existir no catálogo, ou se manager ou code forem NULL.
 * 
 */
const Airport* get_airport_by_code(AirportCatalog* manager, char* code);

/**
 * @brief Obtém o número total de aeroportos no catálogo.
 * 
 * Esta função retorna a quantidade total de aeroportos armazenados no catálogo,
 * consultando o tamanho da tabela hash que os contém.
 * 
 * @param manager Ponteiro para o catálogo de aeroportos.
 * 
 * @return Número total de aeroportos no catálogo, ou 0 se manager for NULL.
 * 
 */
int airport_catalog_get_count(AirportCatalog* manager);

const Airport* get_airport_from_data(const AirportData* data);

int airport_get_arrival_count(AirportCatalog* manager, char* code);
int airport_get_departure_count(AirportCatalog* manager, char* code);

#endif
