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
 * Aloca dinamicamente memória para a estrutura Reservation e inicializa
 * todos os campos com os valores fornecidos.
 *
 * @param reservation_id Identificador único da reserva.
 * @param flight_ids Array de strings contendo os IDs dos voos (tamanho fixo 2).
 * @param document_number Número de documento do passageiro.
 * @param seat Lugar reservado para o passageiro.
 * @param price Preço da reserva.
 * @param extra_luggage Indica se há bagagem extra.
 * @param priority_boarding Indica se há prioridade de embarque.
 *
 * @return Ponteiro para a estrutura Reservation criada, ou NULL se ocorrer erro
 *         na alocação de memória.
 */
Reservation* create_reservation(
    const char* reservation_id,
    char** flight_ids,
    const char* document_number,
    const char* seat,
    double price,
    bool extra_luggage,
    bool priority_boarding
);

/**
 * @brief Liberta a memória associada a uma reserva.
 *
 * Liberta todos os campos dinamicamente alocados e a própria estrutura.
 *
 * @param r Ponteiro para a estrutura Reservation a destruir.
 *
 * @return void
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
