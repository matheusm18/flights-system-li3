#ifndef AIRPORT_PARSER_H
#define AIRPORT_PARSER_H

void remove_quotation_marks(char *s);
void read_csv(const char *filename, void (*callback)(char **fields, int num_fields));

#endif
