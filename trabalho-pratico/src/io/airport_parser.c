#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "io/airport_parser.h"
#include "entities/airport.h"

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 8

void remove_quotation_marks(char *s) {
    if (!s) return;
    
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        for (size_t i = 1; i < len - 1; i++) {
            s[i - 1] = s[i];
        }
        s[len - 2] = '\0';
    }
}

void read_csv(const char *filename, void (*callback)(char **fields, int num_fields)) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int first_line = 1;

    while (fgets(line, sizeof(line), fp)) {
        if (first_line) {
            first_line = 0;
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0';

        char *fields[MAX_FIELDS];
        int num_fields = 0;
        char *token = strtok(line, "\"");

        while (token && num_fields < MAX_FIELDS) {
            if (!(token[0] == ',' && token[1] == '\0')) {
                remove_quotation_marks(token);
                fields[num_fields++] = token;
            }
            token = strtok(NULL, "\"");
        }

        if (callback)
            callback(fields, num_fields);
    }

    fclose(fp);
}