#include "io/output_writer.h"
#include <stdio.h>
#include <stdlib.h>

void write_result(const QueryResult* res, const char* output_path, char delimiter) {
    if (res == NULL) return;

    FILE* f;
    if (output_path == NULL) f = stdout; 
    else {
        f = fopen(output_path, "w");
        if (f == NULL) {
            perror("Erro ao abrir ficheiro de output");
            return;
        }
    }

    int total_lines = get_result_num_lines(res);

    for (int i = 0; i < total_lines; i++) {
        
        int num_tokens = get_line_num_tokens(res, i);

        for (int j = 0; j < num_tokens; j++) {
            const char* token = get_result_token(res, i, j);
            
            if (token != NULL) {
                fprintf(f, "%s", token);
            }

            // coloca o delimitador se NÃO for o ultimo token
            if (j < num_tokens - 1) {
                fprintf(f, "%c", delimiter);
            }
        }
        
        fprintf(f, "\n");
    }

    if (output_path != NULL) {
        fclose(f);
    }
}