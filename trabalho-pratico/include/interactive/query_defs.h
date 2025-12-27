#ifndef QUERY_DEFS_H
#define QUERY_DEFS_H

typedef struct Query Query;
typedef struct query_arg QueryArg;

int get_total_queries();
const Query* get_query_at_index(int index);
int get_query_id(const Query* q);
const char* get_query_nome(const Query* q);
const char* get_query_descricao(const Query* q);
int get_query_permite_s(const Query* q);
int get_query_num_args(const Query* q);
const QueryArg* get_query_arg_at(const Query* q, int index);
const char* get_arg_nome(const QueryArg* arg);
int get_arg_obrigatorio(const QueryArg* arg);

#endif