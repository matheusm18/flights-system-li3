# Compilador
CC = gcc

# Flags de linkagem
LDFLAGS = `pkg-config --libs glib-2.0` -lm

# Flags de compilação
CFLAGS = -Wall -Iinclude `pkg-config --cflags glib-2.0`

# Nome do executável
NAME = programa-principal

# Ficheiros fonte
SRCS = src/main.c src/io/parser.c src/utils/utils.c src/utils/validation.c src/utils/aircraft_validation.c src/utils/airport_validation.c src/utils/date_validation.c\
       src/utils/date.c src/utils/flight_validation.c src/utils/passenger_validation.c src/utils/reservation_validation.c\
       src/catalog/airport_catalog.c src/catalog/flight_catalog.c src/catalog/aircraft_catalog.c \
       src/entities/airport.c src/entities/flight.c src/entities/aircraft.c \
       src/queries/query1.c src/queries/query2.c src/queries/query3.c src/io/command_processor.c

# Ficheiros objeto
OBJS = $(SRCS:.c=.o)

# Regra principal (executável)
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Regras de compilação de cada ficheiro .c para .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpeza (remove executável e object files)
clean:
	rm -f $(NAME) src/*.o src/**/*.o