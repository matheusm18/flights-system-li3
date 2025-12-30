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
 * O catálogo de reservas gere o registo de identificadores de reservas para
 * efeitos de validação de existência, bem como mantém estatísticas
 * agregadas por nacionalidade e por semana para responder às queries.
 *
 * @note Esta estrutura foi otimizada para armazenar apenas as chaves (IDs)
 * e não as estruturas de dados completas.
 */
typedef struct reservation_catalog ReservationCatalog;

/**
 * @brief Cria e inicializa um catálogo de reservas.
 *
 * Esta função aloca memória para um novo catálogo e inicializa
 * as tabelas hash necessárias para armazenar os identificadores de reserva
 * e as estruturas de estatísticas.
 *
 * @return Ponteiro para o catálogo criado, ou NULL em caso de falha.
 */
ReservationCatalog* reservation_catalog_create();

/**
 * @brief Destrói um catálogo de reservas e liberta a memória.
 *
 * Liberta as tabelas hash internas (incluindo as strings dos identificadores
 * de reserva armazenados) e todas as estruturas auxiliares de estatísticas.
 *
 * @param manager Ponteiro para o catálogo a destruir.
 *
 * @return void
 */
void reservation_catalog_destroy(ReservationCatalog* manager);

/**
 * @brief Regista um identificador de reserva no catálogo.
 *
 * Insere o ID da reserva na tabela de referência. Esta função armazena
 * apenas a chave (string) para permitir validações futuras de existência
 * sem guardar a estrutura Reservation completa, visando a
 * otimização de memória.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param reservation_id String com o identificador único da reserva.
 *
 * @return void
 */
void reservation_catalog_add(ReservationCatalog* manager, char* reservation_id);

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
 * @brief Verifica se uma reserva existe no catálogo.
 *
 * Consulta a tabela de IDs de reserva para confirmar se o identificador
 * fornecido é válido e está registado no sistema.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param reservation_id String com o identificador da reserva a verificar.
 *
 * @return true se o ID da reserva existir no catálogo, false caso contrário.
 */
bool reservation_exists(
    ReservationCatalog* manager, 
    char* reservation_id
);

#endif
