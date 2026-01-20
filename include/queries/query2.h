#ifndef QUERY2_H
#define QUERY2_H

#include "queries/query_result.h"
#include "catalog/flight_catalog.h"
#include "catalog/aircraft_catalog.h"
#include "entities/aircraft.h"
#include "utils/query2_utils.h"
#include <stdio.h>

/**
 * @brief Executa a Query 2: Top N aeronaves com mais voos.
 *
 * Esta query devolve um ranking das N aeronaves com maior número de voos.
 * Opcionalmente, pode filtrar apenas por um fabricante específico.
 *
 * Cada linha do resultado contém os seguintes campos:
 *  - Identificador da aeronave
 *  - Fabricante
 *  - Modelo
 *  - Número de voos
 *
 * @param aircraft_manager Ponteiro para o catálogo de aeronaves.
 * @param n Número máximo de aeronaves a incluir no resultado.
 * @param manufacturer Se não for NULL, filtra apenas aeronaves deste fabricante.
 * @return Ponteiro para QueryResult contendo até N linhas de resultado.
 */
QueryResult* execute_query2(AircraftCatalog* aircraft_manager, int n, char* manufacturer);


#endif
