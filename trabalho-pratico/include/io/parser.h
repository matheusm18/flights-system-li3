#ifndef PARSER_H
#define PARSER_H

#include "catalog/catalog_manager.h"

void remove_quotation_marks(char *s);
void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields, void* user_data), void* user_data);
void load_datasets(const char* dataset_path, CatalogManager* catalog_manager);

#endif