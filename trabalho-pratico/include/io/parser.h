#ifndef PARSER_H
#define PARSER_H

#include "catalog/catalog_manager.h"
#include <stdio.h>

void parse_csv_line(char *line, char *fields[], int max_fields, int *num_fields);
void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields, void* user_data, FILE* errors_file), void* user_data, FILE* errors_file);
void load_csv_file(const char* path, int num_fields,
                   void (*process_line)(char **fields, int num_fields, void* user_data, FILE* errors_file),
                   CatalogManager* catalog_manager,
                   const char* errors_file_name,
                   void (*init_errors_file)());
void load_datasets(const char* dataset_path, CatalogManager* catalog_manager);

#endif