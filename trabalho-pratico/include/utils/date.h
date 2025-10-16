#ifndef DATE_H
#define DATE_H

typedef struct date Date;
typedef struct datetime DateTime;

Date* date_create(int year, int month, int day);
void date_destroy(Date* date);

int date_get_day(const Date* d);
int date_get_month(const Date* d);
int date_get_year(const Date* d);

int date_compare(const Date* date1, const Date* date2);

DateTime* datetime_create(int year, int month, int day, int hour, int minute);
void datetime_destroy(DateTime* datetime);

#endif