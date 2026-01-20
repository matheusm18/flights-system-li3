#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "catalog/catalog_manager.h"
#include "queries/query_result.h"
#include <ncurses.h>

/**
 * @brief Executa uma única linha de comando (query).
 *
 * Esta função recebe uma linha de texto contendo uma query,
 * faz o parsing do identificador da query, dos seus argumentos
 * e executa a query correspondente sobre os catálogos.
 *
 * Dependendo do modo de execução, o resultado pode ser:
 *  - escrito para um ficheiro (modo batch)
 *  - apresentado numa janela ncurses (modo interativo)
 *
 * @param line Linha de texto com o comando/query a executar.
 * @param catalog_manager Ponteiro para o gestor de catálogos.
 * @param command_counter Número do comando (usado para nomear ficheiros de output).
 * @param in_ncurses Indica se a execução está em modo ncurses (1) ou modo normal (0).
 * @param win Janela ncurses onde o output será apresentado (se aplicável).
 *
 * @return Ponteiro para a estrutura QueryResult com o resultado da query,
 *         ou NULL se ocorrer erro ou se a query não produzir resultado.
 */
QueryResult* execute_single_line(char* line,
                                 CatalogManager* catalog_manager,
                                 int command_counter,
                                 bool in_ncurses,
                                 WINDOW* win);

/**
 * @brief Processa um ficheiro de comandos em modo batch.
 *
 * Lê um ficheiro de texto contendo várias queries (uma por linha),
 * executa-as sequencialmente e escreve os resultados para ficheiros
 * individuais na pasta de resultados.
 *
 * Cada comando incrementa o contador de comandos, garantindo
 * nomes únicos para os ficheiros de output.
 *
 * @param commands_file Caminho para o ficheiro de comandos.
 * @param catalog_manager Ponteiro para o gestor de catálogos.
 * @param command_counter Ponteiro para o contador de comandos executados.
 */
void process_commands(const char* commands_file,
                      CatalogManager* catalog_manager,
                      int* command_counter);

#endif
