#include <iostream>
#include <string>
#include "Vector.h"
#include <sstream>

using namespace std;

class Date{
private:
    int day;
    int month;
    int year;

public:
    // Constructors
    Date();
    Date(string);
    Date(int, int, int);

    void setDay(int);
    void setMonth(int);
    void setYear(int);

    int getDay();
    int getMonth();
    int getYear();

    void Display();
};


