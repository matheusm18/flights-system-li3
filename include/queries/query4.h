#ifndef QUERY4_H
#define QUERY4_H

#include "queries/query_result.h"
#include "catalog/reservation_catalog.h"
#include "catalog/passenger_catalog.h"

/**
 * @brief Executa a Query 4: Determina o passageiro que esteve mais tempo no Top 10 semanal durante um período.
 *
 * Esta query analisa todas as reservas e calcula qual passageiro permaneceu
 * por mais semanas no Top 10 semanal dentro do intervalo de datas fornecido.
 *
 * Cada linha do resultado contém os seguintes campos:
 *  - ID do passageiro
 *  - Primeiro nome
 *  - Último nome
 *  - Data de nascimento (YYYY-MM-DD)
 *  - Nacionalidade
 *  - Número de semanas no Top 10
 *
 * @param passenger_catalog Ponteiro para o catálogo de passageiros.
 * @param begin_date Data inicial do período (formato "YYYY-MM-DD"). Se NULL, considera todas as datas.
 * @param end_date Data final do período (formato "YYYY-MM-DD"). Se NULL, considera todas as datas.
 * @return Ponteiro para QueryResult contendo os tokens da linha do passageiro vencedor.
 *         Retorna um QueryResult vazio se não houver passageiros ou se os parâmetros forem inválidos.
 */
QueryResult* execute_query4(PassengerCatalog* passenger_catalog, char* begin_date, char* end_date);

#endif
