#ifndef QUERY5_H
#define QUERY5_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "utils/validation_utils.h"
#include <stdbool.h>

/**
 * @brief Prepara as estatísticas das companhias aéreas para ordenação.
 *
 * Esta função percorre todas as estatísticas de voos atrasados por companhia aérea,
 * calcula a média de atraso por voo atrasado e arredonda para 3 casas decimais.
 *
 * @param flight_manager Ponteiro para o catálogo de voos.
 */
void prepare_airline_stats_for_sorting(FlightCatalog* flight_manager);

/**
 * @brief Executa a Query 5: Top N companhias aéreas com maior média de atraso.
 *
 * Esta query retorna as N companhias aéreas com maior média de atraso por voo atrasado.
 *
 * Cada linha do resultado contém os seguintes campos:
 *  - Nome da companhia aérea
 *  - Número de voos atrasados
 *  - Média de atraso (em minutos), arredondada para 3 casas decimais
 *
 * @param flight_manager Ponteiro para o catálogo de voos.
 * @param N Número de companhias a incluir no Top N.
 * @return Ponteiro para QueryResult contendo as linhas com os dados das companhias aéreas.
 *         Retorna um QueryResult vazio se não houver dados ou se N <= 0.
 */
QueryResult* execute_query5(FlightCatalog* flight_manager, int N);

#endif
