#ifndef PARSER_H
#define PARSER_H

#include "catalog/catalog_manager.h"
#include <stdio.h>

void remove_quotation_marks(char *s);
void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields, void* user_data, FILE* errors_file), void* user_data, FILE* errors_file);
void load_datasets(const char* dataset_path, CatalogManager* catalog_manager);

#endif