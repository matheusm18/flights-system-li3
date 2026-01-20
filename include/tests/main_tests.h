#ifndef MAIN_TESTS_H
#define MAIN_TESTS_H

/**
 * @struct discrepancy
 * @brief Representa uma discrepância entre o output gerado e o output esperado.
 * 
 * Campos:
 *  - query: Número da query (1 a NUM_QUERIES) em que ocorreu a discrepância.
 *  - command_id: Número do comando/linha de input onde ocorreu o erro.
 *  - line_error: Número da linha do ficheiro de output onde ocorreu a primeira diferença.
 *  - file_name: Nome do ficheiro de output onde a discrepância foi encontrada.
 */
typedef struct discrepancy Discrepancy;

/**
 * @struct query_stats
 * @brief Estrutura para armazenar estatísticas de execução de cada query.
 * 
 * Campos:
 *  - total_tests: Número total de comandos/testes executados para esta query.
 *  - passed_tests: Número de comandos/testes que retornaram resultados corretos.
 *  - total_time_ms: Tempo total de execução desta query, em milissegundos.
 */
typedef struct query_stats QueryStats;

#include "catalog/catalog_manager.h"

/**
 * @brief Carrega os datasets a partir da pasta indicada e retorna o tempo total de carregamento em milissegundos.
 * 
 * @param csv_files_path Caminho da pasta onde se encontram os ficheiros CSV.
 * @param catalog_manager Ponteiro para o CatalogManager onde os dados serão carregados.
 * @return Tempo de carregamento em milissegundos.
 */
int get_load_dataset_time (const char* csv_files_path, CatalogManager* catalog_manager);

/**
 * @brief Cria ficheiros temporários de comandos agrupados por query, e conta quantos comandos existem para cada query.
 * 
 * Cada ficheiro é nomeado "q<query_number>_commands.txt".
 * 
 * @param input_path Caminho para o ficheiro de input contendo todos os comandos.
 * @param query_counts Array (1-indexed) onde será armazenada a contagem de comandos por query.
 */
void create_query_files(const char *input_path, int *query_counts);

/**
 * @brief Compara dois ficheiros linha a linha, verificando se são iguais.
 * 
 * @param actual_file Caminho para o ficheiro gerado pela execução do programa.
 * @param expected_file Caminho para o ficheiro de output esperado.
 * @param line_error Ponteiro para inteiro que recebe a linha onde ocorreu a primeira diferença (se houver).
 * @return 1 se os ficheiros forem iguais, 0 caso contrário.
 */
int compare_files(const char* actual_file, const char* expected_file, int *line_error);

/**
 * @brief Executa os grupos de comandos de cada query e mede o tempo de execução.
 * 
 * Os tempos e contagens são armazenados na estrutura QueryStats.
 * 
 * @param csv_files_path Caminho da pasta onde se encontram os ficheiros CSV.
 * @param query_counts Array com a contagem de comandos por query.
 * @param stats Array de QueryStats onde serão armazenadas as estatísticas de execução.
 * @param catalog_manager Ponteiro para o CatalogManager que contém os dados carregados.
 */
void execute_query_groups(const char* csv_files_path, int *query_counts, QueryStats *stats, CatalogManager* catalog_manager);

/**
 * @brief Compara todos os outputs gerados com os ficheiros de output esperados.
 * 
 * Regista qualquer discrepância encontrada na array de Discrepancy.
 * 
 * @param expected_output_path Caminho da pasta com os outputs esperados.
 * @param query_counts Array com a contagem de comandos por query.
 * @param stats Array de QueryStats onde os testes bem-sucedidos serão contabilizados.
 * @param discrepancies Array onde serão armazenadas as discrepâncias encontradas.
 * @param discrepancy_count Ponteiro para inteiro que receberá o número total de discrepâncias registadas.
 */
void compare_all_outputs(const char* expected_output_path, int *query_counts, QueryStats *stats, Discrepancy *discrepancies, int *discrepancy_count);

#endif
