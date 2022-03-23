#include <iostream>
#include "Time.h"
#include <cstring>
#include <string>
#include "Vector.h"
#include <sstream>

using namespace std;

void Time::setHour(int h){
    if (h >= 1 && h <= 12){
        this->hr = h;
    }
    else{
        cout << "Invalid Hour" << endl;
    }
}
void Time::setMin(int m){
    if (m >= 0 && m <= 59){
        this->min = m;
    }
    else{
        cout << "Invalid Minutes" << endl;
    }
}
void Time::setSec(int s){
    if (s >= 0 && s <= 59){
        this->sec = s;
    }
    else{
        cout << "Invalid Seconds" << endl;
    }
}
void Time::setPeriod(string p){
    if (p == "am" || p == "pm"){
        this->period = p;
    }
    else{
        cout << "Invalid Period" << endl;
    }
}
void Time::Display(){
    cout << hr << ":" << min << ":" << sec << " " << period << endl;
}

// Constructors
Time::Time(){
    this->hr = 0;
    this->min = 0;
    this->sec = 0;
}
Time::Time(int h, int m, int s, string p){
    this->hr = h;
    this->min = m;
    this->sec = s;
    this->period = p;
}
Time::Time(string time, string period){
    // format of string expected to be hh:mm:ss
    // period refers to am/pm

    // split the time by ':'
    stringstream s(time);
    Vector<string> tempTime;
    string temp;
    while (getline(s, temp, ':')){
        tempTime.pushBack(temp);
    }
    this->hr = stoi(tempTime.get(0));
    this->min = stoi(tempTime.get(1));
    this->sec = stoi(tempTime.get(2));
    this->period = period;
}
