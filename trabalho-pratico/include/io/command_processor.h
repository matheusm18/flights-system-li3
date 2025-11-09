#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "catalog/catalog_manager.h"

void process_commands(const char* commands_file, CatalogManager* catalog_manager, int* command_counter);

#endif