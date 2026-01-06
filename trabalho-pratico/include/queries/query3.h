#ifndef QUERY3_H
#define QUERY3_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h"
#include "utils/validation_utils.h"
#include <stdbool.h>

/**
 * @brief Executa a Query 3: Determina o aeroporto com mais partidas num intervalo de datas.
 *
 * Esta query percorre todos os aeroportos do catálogo e calcula o número de partidas
 * entre as datas especificadas (inclusive). Retorna apenas o aeroporto com o maior número
 * de partidas. Em caso de empate, o aeroporto com o código lexicograficamente menor é escolhido.
 *
 * Cada linha do resultado contém os seguintes campos:
 *  - Código do aeroporto
 *  - Nome do aeroporto
 *  - Cidade
 *  - País
 *  - Número de partidas no intervalo
 *
 * @param airport_manager Ponteiro para o catálogo de aeroportos.
 * @param start_date_str Data inicial do intervalo, no formato "YYYY-MM-DD".
 * @param end_date_str Data final do intervalo, no formato "YYYY-MM-DD".
 * @return Ponteiro para QueryResult com a linha do aeroporto vencedor.
 *         Retorna NULL se algum dos parâmetros for inválido.
 */
QueryResult* execute_query3(AirportCatalog* airport_manager, char* start_date_str, char* end_date_str);

#endif
