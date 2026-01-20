#ifndef RESERVATION_CATALOG_H
#define RESERVATION_CATALOG_H

#include <glib.h>
#include <stdbool.h>
#include "entities/reservation.h"

/**
 * @typedef ReservationStatsIter
 * @brief Iterador para percorrer estatísticas de reservas por nacionalidade.
 *
 * Permite iterar sobre os aeroportos associados a uma determinada nacionalidade
 * e obter o número de reservas correspondentes.
 */
typedef struct reservation_stats_iterator ReservationStatsIter;

/**
 * @typedef ReservationCatalog
 * @brief Estrutura opaca que representa o catálogo de reservas.
 *
 * O catálogo de reservas gere todas as reservas do sistema, bem como estatísticas
 * agregadas por nacionalidade e por semana, suportando várias queries.
 */
typedef struct reservation_catalog ReservationCatalog;

/**
 * @brief Cria e inicializa um catálogo de reservas.
 *
 * Esta função aloca memória para um novo catálogo de reservas e inicializa
 * todas as estruturas internas necessárias, incluindo tabelas hash para
 * armazenar reservas e estatísticas.
 *
 * @return Ponteiro para o catálogo de reservas criado, ou NULL em caso de falha
 *         na alocação de memória.
 */
ReservationCatalog* reservation_catalog_create();

/**
 * @brief Destrói um catálogo de reservas e liberta toda a memória associada.
 *
 * Esta função liberta todas as tabelas hash internas, estruturas auxiliares
 * e reservas armazenadas no catálogo.
 *
 * @param manager Ponteiro para o catálogo de reservas a ser destruído.
 *
 * @return void
 */
void reservation_catalog_destroy(ReservationCatalog* manager);

/**
 * @brief Adiciona uma reserva ao catálogo.
 *
 * Insere uma reserva no catálogo, associando o seu identificador único
 * à estrutura Reservation correspondente.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param reservation Ponteiro para a estrutura Reservation a adicionar.
 *
 * @return void
 *
 * @note Se manager ou reservation forem NULL, a função não realiza nenhuma operação.
 */
void reservation_catalog_add(ReservationCatalog* manager, Reservation* reservation);

/**
 * @brief Incrementa o número de reservas associadas a uma nacionalidade e aeroporto.
 *
 * Atualiza as estatísticas de reservas, incrementando o contador associado
 * a um determinado aeroporto para uma nacionalidade específica.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param nationality Nacionalidade do passageiro.
 * @param airport_id Identificador do aeroporto.
 *
 * @return void
 */
void reservation_catalog_add_nationality_increment(
    ReservationCatalog* manager,
    const char* nationality,
    const char* airport_id
);

/**
 * @brief Cria um iterador para estatísticas de reservas por nacionalidade.
 *
 * Esta função cria um iterador que permite percorrer os aeroportos associados
 * a uma determinada nacionalidade e obter o número de reservas em cada um.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param nationality Nacionalidade cujas estatísticas se pretendem consultar.
 *
 * @return Ponteiro para um iterador de estatísticas, ou NULL se não existirem
 *         dados para a nacionalidade indicada.
 */
ReservationStatsIter* reservation_catalog_create_stats_iter(
    ReservationCatalog* manager,
    const char* nationality
);

/**
 * @brief Avança o iterador de estatísticas para o próximo elemento.
 *
 * Obtém o próximo par (airport_id, count) associado à nacionalidade
 * configurada no iterador.
 *
 * @param it Ponteiro para o iterador de estatísticas.
 * @param airport_id Ponteiro onde será armazenado o ID do aeroporto.
 * @param count Ponteiro onde será armazenado o número de reservas.
 *
 * @return 1 se existir um próximo elemento, 0 se o iterador atingir o fim.
 */
int reservation_catalog_stats_iter_next(
    ReservationStatsIter* it,
    const char** airport_id,
    int* count
);

/**
 * @brief Destrói um iterador de estatísticas de reservas.
 *
 * Liberta a memória associada ao iterador.
 *
 * @param it Ponteiro para o iterador a ser destruído.
 *
 * @return void
 */
void reservation_catalog_stats_iter_destroy(ReservationStatsIter* it);

/**
 * @brief Obtém uma reserva a partir do seu identificador.
 *
 * Procura no catálogo uma reserva com o identificador fornecido.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param reservation_id Identificador da reserva.
 *
 * @return Ponteiro para a estrutura Reservation se existir, ou NULL caso contrário.
 */
Reservation* get_reservation_by_id(
    ReservationCatalog* manager,
    char* reservation_id
);

#endif
