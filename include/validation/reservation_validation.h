#ifndef RESERVATION_VALIDATION_H
#define RESERVATION_VALIDATION_H

#include "catalog/flight_catalog.h"
#include "catalog/passenger_catalog.h"
#include <stdbool.h>

/**
 * @brief Valida se uma string representa um identificador de reserva válido.
 * 
 * Esta função verifica se o identificador de reserva fornecido segue o formato
 * esperado: uma letra 'R' seguida de exatamente 9 dígitos numéricos, totalizando
 * 10 caracteres (ex: "R123456789").
 * 
 * @param reservation_id String que contêm o identificador de reserva a ser validado.
 * 
 * @return true se o ID de reserva é válido (10 caracteres, começa com 'R' seguido
 *         de 9 dígitos).
 * @return false se o ID é NULL, não tem 10 caracteres, não começa com 'R', ou
 *         contém caracteres não numéricos após o 'R'.
 */
bool validate_reservation_id(const char *reservation_id);

/**
 * @brief Valida uma lista de identificadores de voos de uma reserva.
 * 
 * Esta função verifica se os identificadores de voo de uma reserva são válidos
 * e existem no catálogo de voos. Aceita dois formatos:
 * - Voo único: ['AABBCCC'] (11 caracteres)
 * - Voo com conexão: ['AABBCCC', 'DDEEFFF'] (22 caracteres)
 * 
 * Para voos com conexão, também valida se o destino do primeiro voo corresponde
 * à origem do segundo voo (consistência da conexão).
 * 
 * @param flight_ids String que contêm a lista de IDs de voos no formato
 *                   ['ID'] ou ['ID1', 'ID2'].
 * @param manager Ponteiro para o catálogo de voos onde os IDs serão verificados.
 * 
 * @return true se a lista é válida (formato correto, voos existem no catálogo,
 *         e conexão é consistente se aplicável).
 * @return false se flight_ids é NULL, não tem 11 ou 22 caracteres, formato
 *         está incorreto, algum voo não existe, ou a conexão é inconsistente.
 * 
 * @see get_flight_by_flight_id_from_catalog
 * @see flight_catalog_get_destination
 * @see flight_catalog_get_origin
 */
bool validate_flight_ids_reservation(const char *flight_ids, FlightCatalog* manager);

/**
 * @brief Valida se um número de documento existe no catálogo de passageiros.
 * 
 * Esta função verifica se o número de documento fornecido está registado no
 * catálogo de passageiros, garantindo que apenas passageiros conhecidos possam
 * fazer reservas.
 * 
 * @param document_number String que contêm o número de documento a ser validado.
 * @param manager Ponteiro para o catálogo de passageiros onde será realizada
 *                a verificação.
 * 
 * @return true se o número de documento existe no catálogo de passageiros.
 * @return false se o número de documento não existe no catálogo.
 * 
 * @see passenger_catalog_dnumber_exists
 */
bool validate_document_number_reservation(const char *document_number, PassengerCatalog* manager);

#endif