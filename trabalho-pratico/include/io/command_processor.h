#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "catalog/catalog_manager.h"
#include "queries/query_result.h"
#include <ncurses.h>

QueryResult* execute_single_line(char* line, CatalogManager* catalog_manager, int command_counter,bool in_ncurses, WINDOW* win);
void process_commands(const char* commands_file, CatalogManager* catalog_manager, int* command_counter);

#endif