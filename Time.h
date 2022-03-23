#include <iostream>
#include <string>
#include <cstring>

using namespace std;

// This is created for 12 hour clocks
class Time{
private:
    int hr;
    int min;
    int sec;
    string period; // Period refers to am/pm

public:
    Time(int, int, int, string);
    Time(string, string);
    Time();

    void setHour(int);
    void setMin(int);
    void setSec(int);
    void setPeriod(string);

    void Display();
};

