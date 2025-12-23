#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

typedef struct result_line ResultLine;

typedef struct query_result QueryResult;

QueryResult* create_query_result();
void add_line_to_result(QueryResult* res, char** tokens, int num_tokens);
void destroy_query_result(QueryResult* res);

// getters
int get_result_num_lines(const QueryResult* res);
int get_line_num_tokens(const QueryResult* res, int line_idx);
const char* get_result_token(const QueryResult* res, int line_idx, int token_idx);

#endif
