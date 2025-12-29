#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

/**
 * @brief Estrutura que representa uma linha de resultado de uma query.
 *
 * Cada linha é composta por um conjunto de tokens (strings),
 * que correspondem às colunas dessa linha.
 */
typedef struct result_line ResultLine;

/**
 * @brief Estrutura que representa o resultado completo de uma query.
 *
 * Contém um conjunto dinâmico de linhas, cada uma com os seus tokens,
 * bem como informação sobre o número de linhas e a capacidade alocada.
 */
typedef struct query_result QueryResult;

/**
 * @brief Cria e inicializa uma nova estrutura QueryResult.
 *
 * Aloca memória para a estrutura e inicializa um array interno
 * com capacidade inicial para armazenar linhas de resultado.
 *
 * @return Ponteiro para o QueryResult criado, ou NULL em caso de erro.
 */
QueryResult* create_query_result();

/**
 * @brief Adiciona uma linha ao resultado de uma query.
 *
 * A linha é composta por um array de tokens (strings) e pelo número
 * de tokens. O array de tokens passa a ser gerido pelo QueryResult,
 * que ficará responsável por libertar a memória.
 *
 * @param res Ponteiro para o QueryResult.
 * @param tokens Array de strings que representam a linha.
 * @param num_tokens Número de tokens na linha.
 */
void add_line_to_result(QueryResult* res,
                        char** tokens,
                        int num_tokens);

/**
 * @brief Liberta toda a memória associada a um QueryResult.
 *
 * Liberta todas as linhas, os respetivos tokens e a própria estrutura.
 *
 * @param res Ponteiro para o QueryResult a destruir.
 */
void destroy_query_result(QueryResult* res);

/* ===================== GETTERS ===================== */

/**
 * @brief Obtém o número de linhas no resultado.
 *
 * @param res Ponteiro para o QueryResult.
 * @return Número de linhas existentes.
 */
int get_result_num_lines(const QueryResult* res);

/**
 * @brief Obtém o número de tokens de uma linha específica.
 *
 * @param res Ponteiro para o QueryResult.
 * @param line_idx Índice da linha.
 * @return Número de tokens nessa linha, ou 0 se inválido.
 */
int get_line_num_tokens(const QueryResult* res,
                        int line_idx);

/**
 * @brief Obtém um token específico de uma linha.
 *
 * @param res Ponteiro para o QueryResult.
 * @param line_idx Índice da linha.
 * @param token_idx Índice do token dentro da linha.
 * @return Ponteiro para a string do token, ou NULL se inválido.
 */
const char* get_result_token(const QueryResult* res,
                             int line_idx,
                             int token_idx);

#endif
