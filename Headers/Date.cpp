#include "header.h"

Date::Date() : day(1), month(1), year(1) {};

Date::Date(int _day, int _month, int _year)
    : day(_day), month(_month), year(_year) {}

Date::Date(const Date &date)
    : day(date.day), month(date.month), year(date.year) {}

void Date::setDate(int _day, int _month, int _year) {
     this->day = _day;
     this->month = _month;
     this->year = _year;
}

int Date::getDay() const { return this->day; }
int Date::getMonth() const { return this->month; }
int Date::getYear() const { return this->year; }

std::string Date::dateToString() const {
     string day =
         (this->day < 10) ? "0" + to_string(this->day) : to_string(this->day);
     string month = (this->month < 10) ? "0" + to_string(this->month)
                                       : to_string(this->month);
     string year = (this->year < 10) ? "0" + to_string(this->year)
                                     : to_string(this->year);
     return day + '/' + month + '/' + year;
}

ostream &operator<<(ostream &os, const Date &date) {
     os << date.day << '/' << date.month << '/' << date.year;
     return os;
}