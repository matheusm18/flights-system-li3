#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "queries/query_result.h"
#include <stdbool.h>
#include <ncurses.h>

/**
 * @brief Escreve o resultado de uma query.
 *
 * Esta função percorre um QueryResult e escreve o seu conteúdo:
 *  - num ficheiro de output (modo batch),
 *  - no stdout (quando output_path é NULL),
 *  - ou numa janela ncurses (modo interativo).
 *
 * Cada linha do resultado é escrita numa nova linha, com os tokens
 * separados pelo delimitador fornecido.
 *
 * @param res Ponteiro para a estrutura QueryResult (apenas leitura).
 * @param output_path Caminho do ficheiro de output.  
 *        Se for NULL e não estiver em ncurses, escreve no stdout.
 * @param delimiter Caractere separador entre tokens (ex: ';' ou '=').
 * @param in_ncurses Indica se a escrita é feita em modo ncurses (1) ou normal (0).
 * @param win Janela ncurses onde o resultado será escrito (se aplicável).
 */
void write_result(const QueryResult* res,
                  const char* output_path,
                  char delimiter,
                  bool in_ncurses,
                  WINDOW* win);

#endif
