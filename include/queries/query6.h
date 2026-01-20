#ifndef QUERY6_H
#define QUERY6_H

#include "queries/query_result.h"
#include "catalog/reservation_catalog.h"
#include "utils/validation_utils.h"
#include <stdbool.h>

/**
 * @brief Executa a Query 6: Determina o aeroporto de destino mais frequente
 *        para passageiros de uma dada nacionalidade.
 *
 * Esta query percorre todas as reservas do catálogo e identifica o aeroporto
 * com mais passageiros de determinada nacionalidade.
 * 
 * Em caso de empate no número de passageiros, o aeroporto com código lexicograficamente
 * menor é escolhido.
 *
 * Cada linha do resultado contém os seguintes campos:
 *  - Código do aeroporto
 *  - Número de passageiros da nacionalidade especificada
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param nationality Nacionalidade dos passageiros a considerar (string).
 * @return Ponteiro para QueryResult contendo a linha com o aeroporto vencedor.
 *         Retorna um QueryResult vazio se não houver dados ou se parâmetros forem inválidos.
 */
QueryResult* execute_query6(ReservationCatalog* manager, char* nationality);

#endif
