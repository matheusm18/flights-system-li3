#ifndef DATE_H
#define DATE_H

typedef struct date {
    int year;
    int month;
    int day;
} Date;

typedef struct datetime {
    Date date_part;
    int hour;
    int minute;
} DateTime;

Date* date_create(int year, int month, int day);
Date* date_create_from_string(const char* str);
int date_compare(const Date* date1, const Date* date2);

int date_get_day(const Date* d);
int date_get_month(const Date* d);
int date_get_year(const Date* d);

DateTime* datetime_create(int year, int month, int day, int hour, int minute);
DateTime* datetime_create_from_string(const char* str);
int compare_datetime(const DateTime* a, const DateTime* b);

void date_destroy(Date* date);
void datetime_destroy(DateTime* datetime);


int date_get_day(const Date* d);
int date_get_month(const Date* d);
int date_get_year(const Date* d);

#endif