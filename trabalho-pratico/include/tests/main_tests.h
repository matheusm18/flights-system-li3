#ifndef MAIN_TESTS_H
#define MAIN_TESTS_H

typedef struct discrepancy Discrepancy;
typedef struct query_stats QueryStats;

#include "catalog/catalog_manager.h"

int get_load_dataset_time (const char* csv_files_path, CatalogManager* catalog_manager);
void create_query_files(const char *input_path, int *query_counts);
int compare_files(const char* actual_file, const char* expected_file, int *line_error);
void execute_query_groups(const char* csv_files_path, int *query_counts, QueryStats *stats, CatalogManager* catalog_manager);
void compare_all_outputs(const char* expected_output_path, int *query_counts, QueryStats *stats, Discrepancy *discrepancies, int *discrepancy_count);

#endif