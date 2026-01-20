#ifndef FLIGHT_CATALOG_H
#define FLIGHT_CATALOG_H

#include <glib.h>
#include "entities/flight.h"

/**
 * @struct airline_stats
 * @brief Estrutura que armazena estatísticas associadas a uma companhia aérea.
 *
 * Esta estrutura é utilizada para manter informação agregada sobre os atrasos
 * de voos de uma determinada companhia aérea, permitindo o cálculo de métricas
 * como o atraso médio.
 */
typedef struct airline_stats AirlineStats;

/**
 * @struct flight_catalog
 * @brief Estrutura responsável pelo armazenamento e acesso eficiente aos voos.
 *
 * Esta estrutura organiza a informação relativa aos voos e companhias aéreas,
 * utilizando tabelas de hash e arrays dinâmicos para permitir pesquisas
 * rápidas e iteração eficiente.
 */
typedef struct flight_catalog FlightCatalog;

/**
 * @brief Cria e inicializa um catálogo de voos.
 * 
 * Aloca memória para um novo catálogo de voos e inicializa as estruturas
 * internas necessárias:
 *  - Uma tabela hash para armazenar voos indexados pelo flight ID.
 *  - Uma tabela hash auxiliar para estatísticas por companhia aérea.
 *  - Um array dinâmico para armazenar estatísticas das companhias aéreas.
 * 
 * @return Ponteiro para o catálogo criado ou NULL em caso de falha.
 */
FlightCatalog* flight_catalog_create(void);

/**
 * @brief Destrói um catálogo de voos.
 * 
 * Liberta toda a memória associada ao catálogo, incluindo:
 *  - Todos os voos armazenados.
 *  - Estatísticas das companhias aéreas.
 *  - Estruturas auxiliares internas.
 * 
 * @param manager Ponteiro para o catálogo a destruir.
 */
void flight_catalog_destroy(FlightCatalog* manager);

/**
 * @brief Adiciona um voo ao catálogo.
 * 
 * Insere um objeto Flight na tabela hash do catálogo, indexado
 * pelo seu identificador único (flight ID).
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight Ponteiro para o voo a adicionar.
 */
void flight_catalog_add(FlightCatalog* manager, Flight* flight);

/**
 * @brief Inicializa estatísticas para uma companhia aérea.
 * 
 * Cria uma estrutura AirlineStats associada a uma companhia aérea
 * identificada pelo seu código e adiciona-a às estruturas internas
 * do catálogo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param airline_id Identificador da companhia aérea.
 */
void flight_catalog_add_airline_stats(FlightCatalog* manager, char* airline_id);

/**
 * @brief Ordena o array de estatísticas das companhias aéreas.
 * 
 * A ordenação é feita com base na função compare_airlines.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 */
void airline_stats_sort_array(FlightCatalog* manager);

/**
 * @brief Atualiza estatísticas de atraso de uma companhia aérea.
 * 
 * Incrementa o atraso total e o número de voos atrasados
 * associados à companhia aérea indicada.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param airline_id Identificador da companhia aérea.
 * @param delay Valor do atraso a adicionar.
 */
void airline_stats_increment(FlightCatalog* manager, char* airline_id, int delay);

/**
 * @brief Define a média de atraso de uma companhia aérea.
 * 
 * @param s Ponteiro para AirlineStats.
 * @param media Valor da média de atraso.
 */
void set_airline_stats_media(AirlineStats* s, double media);

/**
 * @brief Obtém um voo pelo seu identificador.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight_id Identificador do voo.
 * 
 * @return Ponteiro para o voo ou NULL se não existir.
 */
Flight* get_flight_by_flight_id_from_catalog(FlightCatalog* manager, const char* flight_id);

/**
 * @brief Obtém o aeroporto de origem de um voo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight_id Identificador do voo.
 * 
 * @return Código do aeroporto de origem ou NULL.
 */
char* flight_catalog_get_origin(const FlightCatalog* manager, const char* flight_id);

/**
 * @brief Obtém o aeroporto de destino de um voo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight_id Identificador do voo.
 * 
 * @return Código do aeroporto de destino ou NULL.
 */
char* flight_catalog_get_destination(const FlightCatalog* manager, const char* flight_id);

/**
 * @brief Obtém o número total de voos no catálogo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * 
 * @return Número total de voos.
 */
int flight_catalog_get_count(FlightCatalog* manager);

/**
 * @brief Obtém estatísticas de uma companhia aérea pelo identificador.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param airline_id Identificador da companhia aérea.
 * 
 * @return Ponteiro para AirlineStats ou NULL.
 */
AirlineStats* get_airline_stats_by_identifier(FlightCatalog* manager, char* airline_id);

/**
 * @brief Obtém estatísticas de uma companhia aérea por índice.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param index Índice no array de estatísticas.
 * 
 * @return Ponteiro para AirlineStats ou NULL.
 */
AirlineStats* get_airline_stats_from_array(FlightCatalog* manager, int index);

/**
 * @brief Obtém o número de companhias aéreas com estatísticas registadas.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * 
 * @return Tamanho do array de estatísticas.
 */
int get_airline_stats_array_length(FlightCatalog* manager);

/**
 * @brief Obtém o identificador da companhia aérea.
 * 
 * @param a Ponteiro para AirlineStats.
 * 
 * @return String duplicada com o identificador da companhia aérea.
 */
char* get_airline_stats_airline(AirlineStats* a);

/**
 * @brief Obtém o número de voos atrasados de uma companhia aérea.
 * 
 * @param a Ponteiro para AirlineStats.
 * 
 * @return Número de voos atrasados.
 */
int get_airline_stats_flights_count(AirlineStats* a);

/**
 * @brief Obtém o atraso total acumulado de uma companhia aérea.
 * 
 * @param a Ponteiro para AirlineStats.
 * 
 * @return Atraso total.
 */
long get_airline_stats_total_delay(AirlineStats* a);

/**
 * @brief Obtém a média de atraso de uma companhia aérea.
 * 
 * @param a Ponteiro para AirlineStats.
 * 
 * @return Média de atraso.
 */
double get_airline_stats_media(AirlineStats* a);

#endif
