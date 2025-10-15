#include <utils/validation.h>
#include <stdio.h>
#include <stdlib.h>

bool valid_date(char* date) {
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7) {
            if(date[i] < '0' || date[i] > '9') return false;
        }
    }

    int year, month, day;
    sscanf(date,"%d-%d-%d",&year, &month, &day);

    return (month >= 1 && month <= 12 && day >= 1 && day <= 31);

}