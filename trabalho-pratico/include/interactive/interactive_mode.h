#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include "catalog/catalog_manager.h"

typedef struct Query Query;
typedef struct  query_arg QueryArg;

int largura_menu_queries(Query* queries, int n) ;
int menu_queries(Query* queries, int n) ;
int menu_formato_resultado(int query_id);
void pedir_argumentos(Query q, int com_S, char* buffer, int size) ;
void start_interactive_ui(CatalogManager* manager);
void run_menu_loop(CatalogManager* manager);

#endif