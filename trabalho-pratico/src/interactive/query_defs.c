#include "interactive/query_defs.h"
#include <stddef.h>

#define NUM_QUERIES (int) (sizeof(queries) / sizeof(queries[0]))

struct Query {
    int id;
    const char* nome;
    const char* descricao;
    QueryArg* args;
    int num_args;
    int permite_S; // 1 = sim, 0 = opcional
};

struct query_arg {
    const char* nome;
    int obrigatorio;   // 1 = sim, 0 = opcional
};

static QueryArg q1_args[] = {
    {"ID_AEROPORTO", 1}
};

static QueryArg q2_args[] = {
    {"N", 1},
    {"MANUFACTURER", 0}
};

static QueryArg q3_args[] = {
    {"DATA_INICIO", 1},
    {"DATA_FIM", 1}
};

static QueryArg q4_args[] = {
    {"DATA_INICIO", 0},
    {"DATA_FIM", 0}
};
static QueryArg q5_args[] = {
    {"N", 1}
};

static QueryArg q6_args[] = {
    {"NACIONALIDADE", 1}
};

static Query queries[] = {
    {1, "Query 1", "Resumo de um aeroporto", q1_args, sizeof(q1_args)/sizeof(QueryArg), 1},
    {2, "Query 2", "Top N aeronaves com mais voos", q2_args, sizeof(q2_args)/sizeof(QueryArg), 1},
    {3, "Query 3", "Aeroporto com mais voos entre duas datas", q3_args, sizeof(q3_args)/sizeof(QueryArg), 1},
    {4, "Query 4", "Passageiro mais tempo no Top 10", q4_args, sizeof(q4_args)/sizeof(QueryArg), 1},
    {5, "Query 5", "Top N companhias com mais tempo de atraso medio", q5_args, sizeof(q5_args)/sizeof(QueryArg), 1},
    {6, "Query 6", "Aeroporto de destino mais comum por nacionalidade", q6_args, sizeof(q6_args)/sizeof(QueryArg), 1},
    {0, "Sair", "Terminar programa", NULL, 0, 0}
};

int get_total_queries() {
    return (int)NUM_QUERIES;
}

const Query* get_query_at_index(int index) {
    if (index < 0 || index >= NUM_QUERIES) return NULL;
    return &queries[index];
}

int get_query_id(const Query* q) {
    return q->id;
}

const char* get_query_nome(const Query* q) {
    return q->nome;
}

const char* get_query_descricao(const Query* q) {
    return q->descricao;
}

int get_query_permite_s(const Query* q) {
    return q->permite_S;
}

int get_query_num_args(const Query* q) {
    return q->num_args;
}

const QueryArg* get_query_arg_at(const Query* q, int index) {
    if (!q || !q->args || index < 0 || index >= q->num_args) return NULL;
    return &q->args[index];
}

const char* get_arg_nome(const QueryArg* arg) {
    return arg->nome;
}

int get_arg_obrigatorio(const QueryArg* arg) {
    return arg->obrigatorio;
}