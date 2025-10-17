#ifndef AIRPORT_PARSER_H
#define AIRPORT_PARSER_H

void remover_aspas(char *s);
void ler_csv(const char *filename, void (*callback)(char **campos, int num_campos));

#endif
