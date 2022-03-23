#include <iostream>
#include "Date.h"

using namespace std;

void Date::setDay(int day){
    if (day >= 1 && day <= 31){
        this->day = day;
    }
    else{
        cout << "Invalid Day" << endl;
    }
}

void Date::setMonth(int month){
    if (month >= 1 && month <= 12){
        this->month = month;
    }
    else{
        cout << "Invalid Month" << endl;
    }
}
void Date::setYear(int year){
    if (year >= 1900 && year <= 2100){
        this->year = year;
    }
    else{
        cout << "Invalid Year" << endl;
    }
}

int Date::getDay(){
    return this->day;
}
int Date::getMonth(){
    return this->month;
}
int Date::getYear(){
    return this->year;
}
// Constructors
Date::Date(){
    day = 0;
    month = 0;
    year = 0;
}
Date::Date(int day, int month, int year){
    this->day = day;
    this->month = month;
    this->year = year;
}
Date::Date(string date){
    // format of string expected to be dd/mm/yyyy
    stringstream s(date);
    Vector<string> tempDate;
    string temp;
    while (getline(s, temp, '/')){
        tempDate.pushBack(temp);
    }

    this->day = stoi(tempDate.get(0));
    this->month = stoi(tempDate.get(1));
    this->year = stoi(tempDate.get(2));
}

void Date::Display(){
    cout << day << "-" << month << "-" << year << endl;
}
