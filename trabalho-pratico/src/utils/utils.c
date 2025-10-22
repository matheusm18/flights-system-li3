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