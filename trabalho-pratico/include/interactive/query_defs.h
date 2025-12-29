#ifndef QUERY_DEFS_H
#define QUERY_DEFS_H

/**
 * @brief Estrutura que representa uma Query disponível no sistema.
 *
 * Cada Query contém:
 *  - Um identificador numérico
 *  - Um nome curto
 *  - Uma descrição textual
 *  - Um conjunto de argumentos associados
 *  - Informação sobre se permite o modificador 'S'
 *
 * Esta estrutura é usada apenas para leitura fora deste módulo.
 */
typedef struct Query Query;

/**
 * @brief Estrutura que representa um argumento de uma Query.
 *
 * Cada argumento tem:
 *  - Um nome simbólico (ex.: "DATA_INICIO")
 *  - Um indicador se é obrigatório ou opcional
 */
typedef struct query_arg QueryArg;

/**
 * @brief Obtém o número total de queries disponíveis.
 *
 * @return Número total de queries definidas no sistema.
 */
int get_total_queries();

/**
 * @brief Obtém uma query pelo seu índice no array interno.
 *
 * O índice corresponde à posição da query no menu interativo.
 *
 * @param index Índice da query (0 <= index < total de queries).
 *
 * @return Ponteiro para a Query correspondente ou NULL se o índice for inválido.
 */
const Query* get_query_at_index(int index);

/**
 * @brief Obtém o identificador numérico de uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return Identificador da query (ex.: 1, 2, 3, ...).
 */
int get_query_id(const Query* q);

/**
 * @brief Obtém o nome curto de uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return String com o nome da query.
 */
const char* get_query_nome(const Query* q);

/**
 * @brief Obtém a descrição textual de uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return String com a descrição da query.
 */
const char* get_query_descricao(const Query* q);

/**
 * @brief Indica se a query permite o modificador 'S'.
 *
 * O modificador 'S' altera o formato do output da query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return 1 se permitir 'S', 0 caso contrário.
 */
int get_query_permite_s(const Query* q);

/**
 * @brief Obtém o número de argumentos associados a uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return Número de argumentos da query.
 */
int get_query_num_args(const Query* q);

/**
 * @brief Obtém um argumento específico de uma query.
 *
 * @param q Ponteiro para a Query.
 * @param index Índice do argumento (0 <= index < número de argumentos).
 *
 * @return Ponteiro para o QueryArg correspondente ou NULL se inválido.
 */
const QueryArg* get_query_arg_at(const Query* q, int index);

/**
 * @brief Obtém o nome simbólico de um argumento.
 *
 * @param arg Ponteiro para o argumento da query.
 *
 * @return String com o nome do argumento.
 */
const char* get_arg_nome(const QueryArg* arg);

/**
 * @brief Indica se um argumento é obrigatório.
 *
 * @param arg Ponteiro para o argumento da query.
 *
 * @return 1 se for obrigatório, 0 se for opcional.
 */
int get_arg_obrigatorio(const QueryArg* arg);

#endif
