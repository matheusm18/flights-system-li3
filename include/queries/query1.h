#ifndef QUERY1_H
#define QUERY1_H

#include "queries/query_result.h"
#include "catalog/airport_catalog.h"

/**
 * @brief Executa a Query 1 — Resumo de um aeroporto.
 *
 * Esta query devolve um resumo informativo de um aeroporto,
 * identificado pelo seu código (ex: LIS, OPO, JFK).
 *
 * A linha de resultado contém os seguintes campos, pela ordem:
 *  - Código do aeroporto
 *  - Nome do aeroporto
 *  - Cidade
 *  - País
 *  - Tipo de aeroporto
 *  - Número total de chegadas
 *  - Número total de partidas
 *
 * Caso o aeroporto não exista no catálogo, o resultado é vazio
 * (sem linhas).
 *
 * @param manager Ponteiro para o catálogo de aeroportos.
 * @param airport_code Código identificador do aeroporto.
 * @return Ponteiro para um QueryResult com zero ou uma linha de resultado.
 */
QueryResult* execute_query1(AirportCatalog* manager, char* airport_code);

#endif
