#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "interactive/query_defs.h"
#include <ncurses.h>
#include "validation/validate_arg.h"

/**
 * @file ui_components.h
 * @brief Componentes de interface para o modo interativo.
 *
 * Contém funções para menus, prompts, mensagens de erro e caixas gráficas em ncurses.
 */


/**
 * @brief Calcula a largura do menu de queries automaticamente com base no conteúdo.
 * * Percorre todas as queries registadas no QueryManager para determinar o comprimento 
 * da maior string (ID + Nome + Descrição), garantindo que o menu ncurses 
 * se ajusta visualmente ao conteúdo.
 *
 * @param qm Ponteiro constante para o QueryManager.
 * @return Largura mínima (em colunas) necessária para renderizar o menu.
 */
int largura_menu_queries(const QueryManager* qm);

/**
 * @brief Imprime texto centralizado numa janela.
 * @param w Janela onde imprimir.
 * @param y Linha onde imprimir.
 * @param text Texto a imprimir.
 */
void print_center(WINDOW *w, int y, const char *text);

/**
 * @brief Mostra a tela inicial do programa.
 */
void ui_menu_inicial();

/**
 * @brief Prompt para pedir ao utilizador o caminho do dataset.
 * @return Caminho introduzido pelo utilizador (static buffer, não livre).
 */
char* ui_pedir_caminho_dataset();

/**
 * @brief Renderiza o menu de seleção de queries e processa a entrada do utilizador.
 * * Cria um pad dinâmico em ncurses que lista todas as queries disponíveis no gestor.
 * Permite a navegação com as setas do teclado e seleção com a tecla ENTER.
 *
 * @param qm Ponteiro constante para o QueryManager.
 * @return Índice da query selecionada (0 a num_queries - 1).
 */
int ui_menu_queries(const QueryManager* qm);

/**
 * @brief Menu para selecionar formato de resultado (normal ou 'S') para uma query.
 * @param query_id ID da query.
 * @return 0 = normal, 1 = S, 2 = voltar.
 */
int ui_menu_formato_resultado(int query_id);

/**
 * @brief Prompt para o utilizador introduzir argumentos da query.
 * @param q Ponteiro para a query.
 * @param com_S Se o formato 'S' está ativo (1 = sim, 0 = não).
 * @param buffer Buffer onde guardar os argumentos.
 * @param size Tamanho do buffer.
 */
void ui_pedir_argumentos(const Query* q, int com_S, char* buffer, int size);

/**
 * @brief Menu de aviso para argumentos insuficientes.
 * @param obrigatorios Número de argumentos obrigatórios esperados.
 * @param recebidos Número de argumentos recebidos.
 * @return 0 = tentar novamente, 1 = voltar ao menu.
 */
int ui_menu_aviso_argumentos(int obrigatorios, int recebidos);

/**
 * @brief Mostra mensagem de erro de dataset inválido.
 */
void ui_mostrar_erro_dataset();

/**
 * @brief Mostra janela de carregamento no início.
 * @return Ponteiro para a janela criada (para depois ser fechada).
 */
WINDOW* ui_mostrar_carregamento_inicio();

/**
 * @brief Mostra mensagem de carregamento concluído.
 */
void ui_mostrar_carregamento_fim();

/**
 * @brief Mostra mensagem de erro para argumentos inválidos de uma query.
 * @param res Resultado da validação (ValidationResult).
 * @param escolha Ponteiro onde será armazenada a escolha do utilizador.
 */
void ui_mostrar_erro_arg(ValidationResult *res, int *escolha);

#endif
