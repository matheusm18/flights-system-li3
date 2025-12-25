#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "catalog/catalog_manager.h"

void execute_single_line(char* line, CatalogManager* catalog_manager, int command_counter, int is_interactive);
void process_commands(const char* commands_file, CatalogManager* catalog_manager, int* command_counter);

#endif