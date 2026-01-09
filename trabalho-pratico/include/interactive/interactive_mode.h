#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include "catalog/catalog_manager.h"
#include "interactive/query_defs.h"

#include <ncurses.h>

/**
 * @brief Inicia a interface interativa do programa.
 *
 * Esta função é responsável por:
 *  - Verificar o tamanho do terminal
 *  - Apresentar o menu inicial
 *  - Solicitar e validar o caminho para o dataset
 *  - Verificar a existência dos ficheiros essenciais (flights.csv, passengers.csv, 
 *    reservations.csv, airports.csv, aircrafts.csv)
 *  - Carregar os dados do dataset
 *  - Iniciar o loop principal do menu
 *
 * Se o utilizador não fornecer um caminho, é utilizado o caminho padrão 
 * "dataset-fase-2/sem_erros/". A função valida que o diretório existe e contém 
 * todos os ficheiros necessários antes de prosseguir com o carregamento.
 *
 * @param manager Ponteiro para o gestor de catálogos que armazenará os dados carregados
 * @param qm Ponteiro para o gestor de queries que processará as consultas do utilizador
 */
void start_interactive_ui(CatalogManager* manager, QueryManager* qm);

/**
 * @brief Loop principal do menu interativo de queries.
 *
 * Esta função implementa o ciclo principal da interface interativa, responsável por:
 *  - Apresentar o menu de seleção de queries
 *  - Solicitar e validar o formato de saída (normal ou com flag S)
 *  - Pedir e validar os argumentos obrigatórios e opcionais
 *  - Executar a query selecionada
 *  - Apresentar os resultados numa janela scrollável com suporte a navegação por 
 *    teclado (setas, Page Up/Down) e rato (scroll)
 *  - Permitir retornar ao menu principal
 *
 * A função mantém-se em loop até o utilizador escolher sair. Em caso de argumentos 
 * inválidos, apresenta mensagens de erro e permite reintroduzir os valores ou cancelar.
 * Os resultados são apresentados numa interface ncurses com frame e área de conteúdo 
 * scrollável (pad).
 *
 * @param manager Ponteiro para o gestor de catálogos com os dados carregados
 * @param qm Ponteiro para o gestor de queries que contém as queries disponíveis
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
