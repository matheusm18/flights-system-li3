#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "queries/query_result.h"
#include <stdbool.h>
#include <ncurses.h>

/**
 * @brief Escreve o conteúdo de um QueryResult num ficheiro ou no terminal.
 * * @param res Ponteiro para a estrutura de resultados (apenas leitura).
 * @param output_path Caminho do ficheiro. Se for NULL, escreve no stdout (terminal).
 * @param delimiter Caractere separador (ex: ';' ou ',').
 */
void write_result(const QueryResult* res, const char* output_path, char delimiter, bool in_ncurses, WINDOW* win);

#endif