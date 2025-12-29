#ifndef PARSER_H
#define PARSER_H

#include "catalog/catalog_manager.h"
#include <stdio.h>

/**
 * @brief Faz o parsing de uma linha CSV.
 *
 * A linha é modificada in-place: as aspas de fecho são substituídas por '\0'
 * e cada campo passa a ser apontado diretamente dentro da string original.
 *
 * Assume que todos os campos estão entre aspas e separados por vírgulas.
 *
 * @param line Linha CSV a ser processada (string mutável).
 * @param fields Array onde serão colocados os ponteiros para cada campo.
 * @param max_fields Número máximo de campos esperado.
 * @param num_fields Ponteiro onde será guardado o número de campos lidos.
 */
void parse_csv_line(char *line,
                    char *fields[],
                    int max_fields,
                    int *num_fields);

/**
 * @brief Lê um ficheiro CSV linha a linha e processa cada linha válida.
 *
 * Ignora a primeira linha (header). Para cada linha, faz o parsing e,
 * se o número de campos for o esperado, chama a função callback fornecida.
 *
 * @param fields_length Número de campos esperado por linha.
 * @param filename Caminho do ficheiro CSV.
 * @param callback Função chamada para processar cada linha válida.
 * @param user_data Ponteiro genérico passado para a callback.
 * @param errors_file Ficheiro onde são registados erros de validação.
 */
void read_csv(int fields_length,
              const char *filename,
              void (*callback)(char **fields, int num_fields, void* user_data, FILE* errors_file),
              void* user_data,
              FILE* errors_file);

/**
 * @brief Carrega um ficheiro CSV e inicializa o respetivo ficheiro de erros.
 *
 * Inicializa o ficheiro de erros, abre-o em modo append e lê o CSV,
 * processando cada linha através da função process_line.
 *
 * @param path Caminho do ficheiro CSV.
 * @param num_fields Número de campos esperado por linha.
 * @param process_line Função responsável por validar e inserir os dados.
 * @param catalog_manager Gestor de catálogos usado durante o processamento.
 * @param errors_file_name Caminho do ficheiro de erros.
 * @param init_errors_file Função que inicializa o ficheiro de erros.
 */
void load_csv_file(const char* path,
                   int num_fields,
                   void (*process_line)(char **fields, int num_fields, void* user_data, FILE* errors_file),
                   CatalogManager* catalog_manager,
                   const char* errors_file_name,
                   void (*init_errors_file)());

/**
 * @brief Carrega todos os datasets do sistema.
 *
 * Constrói os caminhos para os ficheiros CSV a partir do diretório base,
 * carrega cada dataset, valida os dados e prepara estruturas auxiliares
 * necessárias para algumas queries.
 *
 * @param dataset_path Diretório onde se encontram os ficheiros CSV.
 * @param catalog_manager Gestor de catálogos a preencher.
 */
void load_datasets(const char* dataset_path,
                   CatalogManager* catalog_manager);

#endif
