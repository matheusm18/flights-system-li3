#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include "catalog/catalog_manager.h"

void start_interactive_ui(CatalogManager* manager);
void run_menu_loop(CatalogManager* manager);

#endif