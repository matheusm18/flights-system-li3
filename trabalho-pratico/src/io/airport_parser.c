#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "io/airport_parser.h"
#include "entities/airport.h"

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 8

void remover_aspas(char *s) {
    if (!s) return;
    
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        for (size_t i = 1; i < len - 1; i++) {
            s[i - 1] = s[i];
        }
        s[len - 2] = '\0';
    }
}

void ler_csv(const char *filename, void (*callback)(char **campos, int num_campos)) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    char linha[MAX_LINE_LENGTH];
    int primeira_linha = 1;

    while (fgets(linha, sizeof(linha), fp)) {
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }

        linha[strcspn(linha, "\r\n")] = '\0';

        char *campos[MAX_FIELDS];
        int num_campos = 0;
        char *token = strtok(linha, "\"");

        while (token && num_campos < MAX_FIELDS) {
            if (!(token[0] == ',' && token[1] == '\0')) {
                remover_aspas(token);
                campos[num_campos++] = token;
            }
            token = strtok(NULL, "\"");
        }

        if (callback)
            callback(campos, num_campos);
    }

    fclose(fp);
}