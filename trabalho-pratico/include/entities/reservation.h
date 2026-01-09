#ifndef RESERVATION_H
#define RESERVATION_H

#include <stdbool.h>

/**
 * @typedef Reservation
 * @brief Estrutura opaca que representa uma reserva de voo.
 *
 * Contém informações relevantes sobre a reserva:
 *  - Identificador da reserva
 *  - IDs dos voos associados
 *  - Número de documento do passageiro
 *  - Lugar reservado
 *  - Preço
 *  - Extra de bagagem
 *  - Prioridade de embarque
 *
 * A implementação concreta encontra-se no ficheiro reservation.c.
 */
typedef struct reservation Reservation;

/**
 * @brief Cria e inicializa uma nova reserva.
 *
 * - `flight_ids`: O array de strings é DUPLICADO internamente.
 * - Outros campos strings: Armazenados por REFERÊNCIA.
 *
 * @param reservation_id ID da reserva (NÃO duplicado; vem da String Pool).
 * @param flight_ids Array de IDs de voo (Serão duplicados).
 * @param document_number Documento do passageiro (NÃO duplicado; vem da String Pool).
 * @param seat Lugar (NÃO duplicado; vem da String Pool).
 * @param price Preço.
 * @param extra_luggage Bagagem extra.
 * @param priority_boarding Prioridade.
 *
 * @return Ponteiro para Reservation ou NULL.
 */
Reservation* create_reservation(const char* reservation_id, char** flight_ids, const char* document_number, const char* seat, double price, bool extra_luggage, bool priority_boarding);

/**
 * @brief Liberta a memória de uma reserva.
 *
 * Liberta a estrutura e o array `flight_ids`.
 * NÃO liberta `reservation_id`, `document_number` ou `seat`.
 *
 * @param r Ponteiro para a reserva.
 */
void destroy_reservation(Reservation* r);

/**
 * @brief Obtém o identificador de uma reserva.
 *
 * @param r Ponteiro para a estrutura Reservation.
 *
 * @return String dinamicamente alocada com o identificador da reserva,
 *         ou NULL se r for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_reservation_identifier(const Reservation* r);

#endif
