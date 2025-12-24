#ifndef UTILS_H
#define UTILS_H
// função nnc é chamada
char *lower_string(const char *str);

/**
 * @brief Converte um int para uma string alocada dinamicamente.
 * @param value O valor inteiro a converter.
 * @return char* Ponteiro para a nova string (o utilizador deve fazer free).
 */
char* int_to_string(int value);

char* double_to_string(double valor);

#endif