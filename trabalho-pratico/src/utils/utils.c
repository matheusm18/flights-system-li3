#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *lower_string(const char *str) {
   char *copy = strdup(str);
   if (!copy) return NULL;

     for (int i = 0; copy[i] != '\0'; i++) {
        copy[i] = tolower((unsigned char) copy[i]);
     }
     return copy;
}

char* int_to_string(int value) {

    char buffer[32];

    // snprintf é seguro porque nunca escreve mais do que o tamanho do buffer
    snprintf(buffer, sizeof(buffer), "%d", value);

    // strdup aloca memória na heap exatamente do tamanho da string gerada
    return strdup(buffer);
}



char* double_to_string(double valor) {

    int tamanho = snprintf(NULL, 0, "%.3f", valor);
    
    char* str = malloc(tamanho + 1);
    
    if (str != NULL) {
        snprintf(str, tamanho + 1, "%.3f", valor);
    }
    
    return str;
}