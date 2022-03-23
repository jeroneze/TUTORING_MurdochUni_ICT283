#include <iostream>
#include "Date.h"
#include "Time.h"
#include <fstream>
#include <cmath>

using namespace std;

void testDate(){
    cout << "TEST DATE" << endl;
    Date date;
    date.setDay(200000); // Invalid Day
    date.Display();
    date.setMonth(02);
    date.setYear(2000);
    date.setDay(12);
    date.Display();

    Date date2(22, 03, 2003);
    date2.Display();
}

void testTime(){
    cout << "TEST TIME" << endl;
    Time time;
    time.setHour(59);
    time.Display();
    time.setHour(12);
    time.setMin(56);
    time.setSec(59);
    time.setPeriod("pm");
    time.Display();

    Time time2(01, 23, 10, "pm");
    time2.Display();
}

// Testing functionality of my vector class to see if it works as expected.
void testVector(){
    Vector<double> doubleVector;
    doubleVector.pushBack(1.2);
    doubleVector.pushBack(2.3);
    doubleVector.pushBack(3.3);
    doubleVector.display();
    doubleVector.push(4.3, 1);
    doubleVector.display();
    doubleVector.pop(2);
    doubleVector.display();
}


typedef struct{
    Date d;
    Time t;
    float windSpeed;
    float solarRadiation;
    float ambientAirTemperature;
} DataLogType;

// This method is fitted specially for this project to increase efficiency as there are huge numbers of data
// rows that needs to be read. Refer to readExcelFile() function below for the initial loosely coupled design
Vector<DataLogType> readExcelFileToDataLog(string filePath){
    // File pointer
    fstream fin;

    // Open the .csv file
    fin.open(filePath, ios::in);

    // This Vector will be used to store the processed value
    Vector<DataLogType> dataLog;

    ifstream myfile(filePath);

    // These values will be used to store the data rows and used for processing
    string line, col, temp;

    if(myfile) {
        int i = 0;
        while (getline(myfile, line)){
            if (i > 0){ // Ignore data row 1 which is the headers
                // Read the data into a String Vector
                Vector<string> row;

                // This is for splitting the string
                stringstream s(line);

                // Split the data row string by comma and store it in col
                while (getline(s, col, ',')){
                    // add all the data columns into the vector
                    row.pushBack(col);
                }

                stringstream ss(row.get(0)); // row.get(0) is the date time entry (WEST)

                Vector<string> tempDateTime;
                string temp;
                // Split the string by ' ' and store each value into the Vector
                while (getline(ss, temp, ' ')){
                    tempDateTime.pushBack(temp);
                }

                Date date(tempDateTime.get(0)); // 0 is the date (dd/mm/yyyy)
                Time time(tempDateTime.get(1), tempDateTime.get(2)); // 1 is the time (hh:mm:ss), 2 is the period (am/pm)

                // Create WindLog entry
                DataLogType datalog_val;
                datalog_val.d = date;
                datalog_val.t = time;

                // Convert wind speed from m/s to km/h
                // 3.6 = (1 * 60 * 60) / 1000
                // (* 60 *60) to convert seconds to hours
                // (/ 100) to convert meter to kilometers
                float windSpeed_kmh = stof(row.get(10)) * 3.6;
                datalog_val.windSpeed = windSpeed_kmh; // 10 is S (Wind speed), stof is convert string to float
                datalog_val.solarRadiation = stof(row.get(11)); // 11 is SR (Solar radiation)
                datalog_val.ambientAirTemperature = stof(row.get(17)); // 17 is T (Ambient Air Temperature)

                // Add WindLog entry to the vector
                dataLog.pushBack(datalog_val);
            }
            i++;
        }
    }
    else{
        cout << "There was a problem reading the .CSV file, please check file path" << endl;
    }
    return dataLog;
}

int getMonthNumFromStr(string month_val){
    Vector<string> months;
    months.pushBack("January");
    months.pushBack("February");
    months.pushBack("March");
    months.pushBack("April");
    months.pushBack("May");
    months.pushBack("June");
    months.pushBack("July");
    months.pushBack("August");
    months.pushBack("September");
    months.pushBack("October");
    months.pushBack("November");
    months.pushBack("December");

    return months.getIndex(month_val) + 1;
}

string getMonthStrFromNum(int month_val){
    Vector<string> months;
    months.pushBack("January");
    months.pushBack("February");
    months.pushBack("March");
    months.pushBack("April");
    months.pushBack("May");
    months.pushBack("June");
    months.pushBack("July");
    months.pushBack("August");
    months.pushBack("September");
    months.pushBack("October");
    months.pushBack("November");
    months.pushBack("December");

    return months.get(month_val - 1);
}

// Function to get the number of days in the month
int getNumberOfDaysInMonth(int month, int year){
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){ // months with 31 days
        return 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11){ // months with 30 days
        return 30;
    }
    else if (month == 2){
        if ((year - 2000) % 4 == 0){ // Check if it is a Leap year
            return 29;
        }
        else{
            return 28;
        }
    }
    else{
        return -1;
    }
}

// Function to find the start and end index of a specified month in the Vector
Vector<int> getIndexsOfMonth(Vector<DataLogType> dataLog, int month, int year){
    Vector<int> indexVec; // This vector is to store the start and end index
    int i = 0;
    while (i<dataLog.length()){
        DataLogType dataLog_val = dataLog.get(i); // This will be the first entry of each month
        if (dataLog_val.d.getYear() == year && dataLog_val.d.getMonth() == month){ // Found the month
            indexVec.pushBack(i);
            if (month == 3){
                if (i < (int)(dataLog.length()/2)){ // First march of data
                    indexVec.pushBack(i+(getNumberOfDaysInMonth(month, year) * 144) - 54); // First March has 54 lesser data
                }
                else { // Second march of data
                    indexVec.pushBack(i + 54); // Second march only has 54 data entries
                }
            }
            else{
                indexVec.pushBack(i+(getNumberOfDaysInMonth(month, year) * 144)); // *144 as 1 day has 144 values
            }
            break;
        }
        else {
            if (dataLog_val.d.getMonth() == 3){
                if (i < (int)(dataLog.length()/2)){ // First march of data
                    i += (getNumberOfDaysInMonth(dataLog_val.d.getMonth(), dataLog_val.d.getYear()) * 144) - 54; // First March has 54 lesser data
                }
                else { // Second march of data
                    i += 54; // Second march only has 54 data entries
                }
            }
            else{
                i += getNumberOfDaysInMonth(dataLog_val.d.getMonth(), dataLog_val.d.getYear()) * 144; // Skip all the days in the current month and go to next month
            }
        }
    }
    return indexVec;
}

// Function to calculate average wind speed and ambient air temperature, given the start index and end index of vector
Vector<float> calculateAverages(Vector<DataLogType> dataLog, int startIndex, int endIndex){
    // Values to store total, will be used to calculate average later
    Vector<float> averageValues;
    float totalWind = 0.0;
    float totalTemperature = 0.0;

    // Add all the values where it is in the month
    for(int i=startIndex; i<endIndex; i++){
        DataLogType dataLog_val = dataLog.get(i);
        totalWind += dataLog_val.windSpeed;
        totalTemperature += dataLog_val.ambientAirTemperature;
    }

    averageValues.pushBack(round((totalWind/(endIndex-startIndex))*10.0)/10.0); // Round to 1 dp
    averageValues.pushBack(round((totalTemperature/(endIndex-startIndex))*10.0)/10.0); // Round to 1 dp

    return averageValues;
}

double calculateTemperature(Vector<DataLogType> dataLog, int startIndex, int endIndex){
    // Add all the values where it is in the month
    double total;
    for(int i=startIndex; i<endIndex; i++){
        DataLogType dataLog_val = dataLog.get(i);
        // Convert to Kwh/m^2 from W/m^2
        double convertedValue = ((double)dataLog_val.ambientAirTemperature/6) / 1000;
        total += convertedValue;
    }

    return round(total*10.0)/10.0; // Round to 1 dp
}

int main()
{
    //testDate();
    //testTime();
    //testVector();

    // Retrieve from .csv file by data rows and columns
    // To test the bigger csv file, Change this string to "data/MetData_Mar01-2014-Mar01-2015-ALL.csv", smaller file is : "data/MetData_31-3a.csv"
    Vector<DataLogType> dataLog = readExcelFileToDataLog("data/MetData_Mar01-2014-Mar01-2015-ALL.csv");

    while(true){ // Program keeps running until user inputs 5
        string option; // Used to store what the user input at the menu
        cout << endl << "Main Menu" << endl <<
                "1. Average wind speed and ambient air temperature for a specific month and year" << endl <<
                "2. Average wind speed and ambient air temperature for each month of a specific year" << endl <<
                "3. Total solar radiation in kWh/m^2 for each month of a specific year" << endl <<
                "4. Average wind speed (km/h), average ambient air temperature and total solar radiation in kWh/m^2 for each month of a specified year" << endl <<
                "5. Exit Program" << endl <<
                "Enter Option: ";
        cin >> option;
        cout << endl;

        if (option == "1"){
            int year, month;
            string strMonth;
            cout << "Enter Month and Year in format (January 1905): ";
            cin >> strMonth >> year;
            month = getMonthNumFromStr(strMonth);

            if (month == 0 || (year < 1900 || year > 2100)){ // Invalid date formats
                cout << "Invalid Date Entered" << endl;
            }
            else{
                Vector<int> indexVec = getIndexsOfMonth(dataLog, month, year); // Retrieve the start and end index values of the month
                cout << strMonth << " " << year << ": ";
                if (indexVec.length() == 0){ // No records were found for the month
                     cout << "No Data" << endl;
                }
                else { // Records found
                    Vector<float> averages = calculateAverages(dataLog, indexVec.get(0), indexVec.get(1));
                    cout << averages.get(0) << " km/h, " << averages.get(1) << " degrees C" << endl;
                }
            }
        }
        else if (option == "2"){
            int year;
            cout << "Enter Year: ";
            cin >> year;
            if (year < 1900 || year > 2100){
                cout << "Invalid Date Entered" << endl;
            }
            else{
                cout << year << endl;
                for (int i=1; i<=12; i++){
                    Vector<int> indexVec = getIndexsOfMonth(dataLog, i, year);
                    cout << getMonthStrFromNum(i) << ": ";
                    if (indexVec.length() == 0){ // No records were found for the month
                         cout << "No Data" << endl;
                    }
                    else { // Records found
                        Vector<float> averages = calculateAverages(dataLog, indexVec.get(0), indexVec.get(1));
                        cout << averages.get(0) << " km/h, " << averages.get(1) << " degrees C" << endl;
                    }
                }
            }

        }
        else if (option == "3"){
            int year;
            cout << "Enter Year: ";
            cin >> year;
            if (year < 1900 || year > 2100){
                cout << "Invalid Date Entered" << endl;
            }
            else{
                cout << year << endl;
                for (int i=1; i<=12; i++){
                    Vector<int> indexVec = getIndexsOfMonth(dataLog, i, year);
                    cout << getMonthStrFromNum(i) << ": ";
                    if (indexVec.length() == 0){ // No records were found for the month
                         cout << "No Data" << endl;
                    }
                    else { // Records found
                        double temperature = calculateTemperature(dataLog, indexVec.get(0), indexVec.get(1));
                        cout << temperature << " kWh/m^2" << endl;
                    }
                }
            }
        }
        else if (option == "4"){
            int year;
            cout << "Enter Year: ";
            cin >> year;
            if (year < 1900 || year > 2100){
                cout << "Invalid Date Entered" << endl;
            }
            else{
                cout << year << endl;
                for (int i=1; i<=12; i++){
                    Vector<int> indexVec = getIndexsOfMonth(dataLog, i, year);
                    cout << getMonthStrFromNum(i) << ", ";
                    if (indexVec.length() == 0){ // No records were found for the month
                         cout << "No Data" << endl;
                    }
                    else { // Records found
                        double temperature = calculateTemperature(dataLog, indexVec.get(0), indexVec.get(1));
                        Vector<float> averages = calculateAverages(dataLog, indexVec.get(0), indexVec.get(1));
                        cout << averages.get(0) << ", " << averages.get(1) << ", " <<
                                temperature << endl;
                    }
                }
            }
        }
        else if (option == "5"){
            cout << "Thank you for using the program" << endl;
            break;
        }
        else{
            cout << "Invalid Option" << endl;
        }
    }

    return 0;
}

// Initial design of readExcelFile, which can be used for all cases as it will only read data and
// split it into data rows and columns. However, it will require double loop as after returning the
// Vector<Vector<string>>, I will still need to loop through it again to create the objects in the
// dataLogType classes. This method is loosely coupled.
Vector<Vector<string>> readExcelFile(string filePath){
    // File pointer
    fstream fin;

    // Open the .csv file
    fin.open(filePath, ios::in);

    // This Vector will be used to store the processed value
    // Each Vector<string> will represent 1 data row
    // Each string will represent 1 data column of that row
    Vector<Vector<string>> processedVectors;
    // These values will be used to store the data rows and used for processing

    ifstream myfile(filePath);

    string line, col, temp;

    if(myfile) {
        while (getline(myfile, line)){
            // Read the data into a String Vector
            Vector<string> row;

            // This is for splitting the string
            stringstream s(line);

            // Split the data row string by comma and store it in col
            while (getline(s, col, ',')){
                // add all the data columns into the vector
                row.pushBack(col);
            }
            // Add the columns vector into another vector
            processedVectors.pushBack(row);
        }
    }
    processedVectors.pop(0); // Remove the headers
    return processedVectors;
}

