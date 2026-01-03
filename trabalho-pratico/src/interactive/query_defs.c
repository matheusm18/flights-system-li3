#include "interactive/query_defs.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define ARRAY_SIZE(arr) (int)(sizeof(arr) / sizeof(arr[0]))

struct query_arg {
    const char* nome;
    int obrigatorio;   // 1 = sim, 0 = opcional
};

struct Query {
    int id;
    const char* nome;
    const char* descricao;
    QueryArg* args;
    int num_args;
    int permite_S; // 1 = sim, 0 = opcional
};

struct query_manager {
    Query* queries;
    int num_queries;
    int capacity;
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

QueryManager* create_query_manager(int initial_capacity) {
    QueryManager* qm = malloc(sizeof(QueryManager));
    qm->queries = malloc(sizeof(Query) * initial_capacity);
    qm->num_queries = 0;
    qm->capacity = initial_capacity;
    return qm;
}

void add_query(QueryManager* qm, int id, const char* nome, const char* desc, QueryArg* args, int n_args, int permite_S) {
    if (qm->num_queries >= qm->capacity) {
        qm->capacity *= 2;
        qm->queries = realloc(qm->queries, sizeof(Query) * qm->capacity);
    }

    Query* q = &qm->queries[qm->num_queries];
    q->id = id;
    q->nome = strdup(nome);      
    q->descricao = strdup(desc); 
    q->args = args;              
    q->num_args = n_args;
    q->permite_S = permite_S;

    qm->num_queries++;
}

void init_all_queries(QueryManager* qm) {
    add_query(qm, 1, "Query 1", "Resumo de um aeroporto", q1_args, ARRAY_SIZE(q1_args), 1);

    add_query(qm, 2, "Query 2", "Top N aeronaves com mais voos", q2_args, ARRAY_SIZE(q2_args), 1);

    add_query(qm, 3, "Query 3", "Aeroporto com mais voos entre duas datas", q3_args, ARRAY_SIZE(q3_args), 1);

    add_query(qm, 4, "Query 4", "Passageiro mais tempo no Top 10", q4_args, ARRAY_SIZE(q4_args), 1);

    add_query(qm, 5, "Query 5", "Top N companhias com mais tempo de atraso medio", q5_args, ARRAY_SIZE(q5_args), 1);

    add_query(qm, 6, "Query 6", "Aeroporto de destino mais comum por nacionalidade", q6_args, ARRAY_SIZE(q6_args), 1);

    add_query(qm, 0, "Sair", "Terminar programa", NULL, 0, 0);
}

void free_query_manager(QueryManager* qm) {
    if (!qm) return;

    for (int i = 0; i < qm->num_queries; i++) {
        free((void*)qm->queries[i].nome);
        free((void*)qm->queries[i].descricao);
    }

    free(qm->queries);
    free(qm);
}


int get_total_queries(const QueryManager* qm) {
    if (!qm) return 0;
    return qm->num_queries;
}

const Query* get_query_at_index(const QueryManager* qm, int index) {
    if (!qm || index < 0 || index >= qm->num_queries) return NULL;
    return &qm->queries[index];
}

int get_query_id(const Query* q) {
    if (!q) return -1;
    return q->id;
}

const char* get_query_nome(const Query* q) {
    if (!q) return "N/A";
    return q->nome;
}

const char* get_query_descricao(const Query* q) {
    if (!q) return "";
    return q->descricao;
}

int get_query_permite_s(const Query* q) {
    if (!q) return 0;
    return q->permite_S;
}

int get_query_num_args(const Query* q) {
    if (!q) return 0;
    return q->num_args;
}

const QueryArg* get_query_arg_at(const Query* q, int index) {
    if (!q || !q->args || index < 0 || index >= q->num_args) return NULL;
    return &q->args[index];
}

const char* get_arg_nome(const QueryArg* arg) {
    if (!arg) return "";
    return arg->nome;
}

int get_arg_obrigatorio(const QueryArg* arg) {
    if (!arg) return 0;
    return arg->obrigatorio;
}