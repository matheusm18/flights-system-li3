#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include "catalog/catalog_manager.h"
#include "interactive/query_defs.h"

#include <ncurses.h>

/**
 * @brief Inicia a interface interativa do programa.
 *
 * Esta função controla todo o fluxo da UI interativa:
 *  - Verifica o tamanho mínimo do terminal
 *  - Pede ao utilizador o caminho para o dataset
 *  - Carrega os datasets no CatalogManager
 *  - Executa o menu de queries de forma interativa
 *
 * @param manager Ponteiro para a estrutura CatalogManager contendo os catálogos
 *        carregados.
 */
void start_interactive_ui(CatalogManager* manager, QueryManager* qm);

/**
 * @brief Loop principal do menu interativo de queries.
 *
 * Permite navegar pelas queries disponíveis, validar argumentos,
 * executar queries e mostrar os resultados numa interface ncurses
 * com scroll e suporte a teclado e rato.
 *
 * @param manager Ponteiro para a estrutura CatalogManager.
 */
void run_menu_loop(CatalogManager* manager, QueryManager* qm);

/**
 * @brief Aguarda até que o terminal tenha tamanho mínimo aceitável.
 *
 * A função bloqueia até que o terminal tenha, pelo menos, MIN_ROWS linhas
 * e MIN_COLS colunas. Durante a espera, é mostrado um aviso para o utilizador.
 */
void wait_for_bigger_terminal_size();


/**
 * @brief Conta o número de palavras (tokens) numa string separadas por espaços.
 *
 * @param str A string a ser analisada.
 * @return O número total de tokens encontrados. Retorna 0 se a string for NULL ou vazia.
 * * @note O limite máximo para análise é de 512 caracteres devido ao tamanho do buffer temporário.
 */
int contar_tokens(const char* str);

#endif
