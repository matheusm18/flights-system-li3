#ifndef RESERVATION_CATALOG_H
#define RESERVATION_CATALOG_H

#include <glib.h>
#include "entities/reservation.h"

/**
 * @typedef WeeklyTop
 * @brief Estrutura opaca que representa os passageiros com maior gasto numa semana.
 *
 * Esta estrutura é utilizada internamente para suportar a Query 4, armazenando
 * o identificador da semana e os IDs dos passageiros com maior gasto nesse período.
 * A sua definição concreta encontra-se no ficheiro de implementação.
 */
typedef struct weekly_top WeeklyTop;

/**
 * @typedef PassengerSpend
 * @brief Estrutura auxiliar para associar um passageiro ao seu valor gasto.
 *
 * Utilizada internamente durante o processamento e ordenação de passageiros
 * com base no total gasto num determinado período.
 */
typedef struct passenger_spend PassengerSpend;

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
 *
 * @note Se manager for NULL, a função não realiza nenhuma operação.
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
 * @brief Atualiza o total gasto por um passageiro numa determinada semana.
 *
 * Esta função incrementa o valor gasto por um passageiro identificado por
 * passenger_id numa semana específica, identificada por weekID.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param weekID Identificador da semana.
 * @param passenger_id Identificador do passageiro.
 * @param price Valor a adicionar ao total gasto.
 *
 * @return void
 */
void reservation_catalog_add_price_increment(
    ReservationCatalog* manager,
    const char* weekID,
    const char* passenger_id,
    double price
);

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

/**
 * @brief Função de comparação de passageiros por valor gasto.
 *
 * Compara dois PassengerSpend, ordenando-os por valor gasto de forma decrescente.
 * Em caso de empate, a ordenação é feita pelo identificador do passageiro.
 *
 * @param a Ponteiro para o primeiro elemento a comparar.
 * @param b Ponteiro para o segundo elemento a comparar.
 *
 * @return Valor negativo, zero ou positivo conforme a ordenação.
 */
int compare_passengers(const void* a, const void* b);

/**
 * @brief Função de comparação de semanas.
 *
 * Compara duas estruturas WeeklyTop com base na chave da semana.
 *
 * @param a Ponteiro para o primeiro elemento a comparar.
 * @param b Ponteiro para o segundo elemento a comparar.
 *
 * @return Valor negativo, zero ou positivo conforme a ordenação.
 */
int compare_weeks(const void* a, const void* b);

/**
 * @brief Função de comparação de strings.
 *
 * Compara duas strings lexicograficamente.
 *
 * @param a Ponteiro para a primeira string.
 * @param b Ponteiro para a segunda string.
 *
 * @return Valor negativo, zero ou positivo conforme a ordenação.
 */
int compare_strings(const void *a, const void *b);

/**
 * @brief Prepara os dados necessários para a Query 4.
 *
 * Processa as estatísticas semanais e constrói uma linha temporal com
 * os passageiros de maior gasto por semana, permitindo consultas eficientes.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 *
 * @return void
 */
void reservation_catalog_prepare_query4(ReservationCatalog* manager);

/**
 * @brief Obtém o passageiro mais frequente no top semanal num dado período.
 *
 * Determina o passageiro que aparece mais vezes no top de passageiros
 * com maior gasto entre duas datas.
 *
 * @param manager Ponteiro para o catálogo de reservas.
 * @param begin_date Data de início do período (string).
 * @param end_date Data de fim do período (string).
 * @param out_count Ponteiro onde será armazenado o número de ocorrências.
 *
 * @return String com o identificador do passageiro vencedor (alocada dinamicamente),
 *         ou NULL se não existirem dados no período indicado.
 */
char* reservation_catalog_get_top_passenger_in_period(
    ReservationCatalog* manager,
    char* begin_date,
    char* end_date,
    int* out_count
);

#endif
