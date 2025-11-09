# Diretórios
SRC_DIR     = src
CATALOG_DIR = $(SRC_DIR)/catalog
ENTITIES_DIR= $(SRC_DIR)/entities
IO_DIR      = $(SRC_DIR)/io
QUERIES_DIR = $(SRC_DIR)/queries
TESTS_DIR   = tests
UTILS_DIR   = $(SRC_DIR)/utils

# Compilador
CC     = gcc
CFLAGS = -Wall -Iinclude `pkg-config --cflags glib-2.0`
LDFLAGS= `pkg-config --libs glib-2.0` -lm

MAIN_PROGRAM = programa-principal
TEST_PROGRAM = programa-testes

# Fontes da aplicação
SRCS_APP     := $(shell find $(SRC_DIR) -name '*.c' ! -name 'main.c')
SRCS_MAIN    := $(SRC_DIR)/main.c
SRCS_TEST    := $(wildcard $(TESTS_DIR)/*.c)

# Objetos
OBJS_APP     := $(SRCS_APP:.c=.o)
OBJS_MAIN    := $(SRCS_MAIN:.c=.o)
OBJS_TEST    := $(SRCS_TEST:.c=.o)

# Alvo padrão
all: $(MAIN_PROGRAM) $(TEST_PROGRAM)

# Programa principal
$(MAIN_PROGRAM): $(OBJS_MAIN) $(OBJS_APP)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Programa de testes
$(TEST_PROGRAM): $(OBJS_TEST) $(OBJS_APP)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Compilar .c -> .o (src)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar .c -> .o (subpastas do src)
$(SRC_DIR)/%/%.o: $(SRC_DIR)/%/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar .c -> .o (tests)
$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(MAIN_PROGRAM) $(TEST_PROGRAM) $(OBJS_APP) $(OBJS_MAIN) $(OBJS_TEST)