#ifndef PARSER_H
#define PARSER_H

void remove_quotation_marks(char *s);
void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields));
void load_datasets(const char* dataset_path);

#endif