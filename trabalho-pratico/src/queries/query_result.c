#include "queries/query_result.h"
#include <stdlib.h>
#include <string.h>

struct result_line {
    char** tokens; // array de tokens
    int num_tokens;
};

struct query_result {
    ResultLine** lines; // array de ponteiros para as linhas
    int num_lines;
    int capacity;
};

QueryResult* create_query_result() {
    QueryResult* res = malloc(sizeof(QueryResult));
    if (res == NULL) return NULL;

    res->num_lines = 0;
    res->capacity = 10; // capacidade inicial de 10 linhas de resultado
    res->lines = malloc(res->capacity * sizeof(ResultLine*));
    
    if (res->lines == NULL) {
        free(res);
        return NULL;
    }

    return res;
}

void add_line_to_result(QueryResult* res, char** tokens, int num_tokens) {
    if (res == NULL || tokens == NULL) return;

    // verificar se atingiu o limite (para fazer resize)
    if (res->num_lines >= res->capacity) {
        int new_capacity = res->capacity * 2;

        ResultLine** new_lines = realloc(res->lines, sizeof(ResultLine*) * new_capacity);
        if (new_lines == NULL) return;

        res->lines = new_lines;
        res->capacity = new_capacity;
    }

    ResultLine* line = malloc(sizeof(ResultLine));
    if (line == NULL) return;

    line->tokens = tokens;
    line->num_tokens = num_tokens;

    res->lines[res->num_lines] = line;
    res->num_lines++;
}

void destroy_query_result(QueryResult* res) {
    if (res == NULL) return;

    for (int i = 0; i < res->num_lines; i++) {
        ResultLine* line = res->lines[i];
        if (line) {
            // liberta cada string (token) individual
            for (int j = 0; j < line->num_tokens; j++) {
                free(line->tokens[j]);
            }
            free(line->tokens); // liberta o array de tokens
            free(line); // liberta a struct da linha
        }
    }
    // liberta o array de linhas e a struct
    free(res->lines);
    free(res);
}


// getters

int get_result_num_lines(const QueryResult* res) {
    if (res == NULL) return 0;

    return res->num_lines;
}

int get_line_num_tokens(const QueryResult* res, int line_idx) {
    if (res == NULL || (line_idx < 0 || line_idx >= res->num_lines)) return 0; 

    return res->lines[line_idx]->num_tokens;
}

const char* get_result_token(const QueryResult* res, int line_idx, int token_idx) {
    if (res == NULL || line_idx < 0 || line_idx >= res->num_lines) return NULL;

    ResultLine* line = res->lines[line_idx];
    if (line == NULL) return NULL;

    if (token_idx < 0 || token_idx >= line->num_tokens) return NULL;

    return res->lines[line_idx]->tokens[token_idx];
}