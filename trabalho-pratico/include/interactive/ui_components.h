#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "interactive/query_defs.h"
#include <ncurses.h>
#include "validation/validate_arg.h"

void ui_menu_inicial();
char* ui_pedir_caminho_dataset();
int ui_menu_queries(int n);
int ui_menu_formato_resultado(int query_id);
void ui_pedir_argumentos(const Query* q, int com_S, char* buffer, int size);
int ui_menu_aviso_argumentos(int obrigatorios, int recebidos);
void ui_mostrar_erro_dataset();
WINDOW* ui_mostrar_carregamento_inicio();
void ui_mostrar_carregamento_fim();
void ui_mostrar_erro_arg(ValidationResult *res, int *escolha);

#endif