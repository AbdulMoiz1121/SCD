#ifndef UTILS_CPP
#define UTILS_CPP

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

// Function declarations and definitions
int getIntInput() {
    int input;
    while (true) {
        if (cin >> input) {
            return input;
        } else {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            cout << "Invalid input! Please enter a numeric value (Integer): ";
        }
    }
}

string intToString(int v) {
    stringstream ss;
    ss << v;
    return ss.str();
}

string toLower(string s) {
    string lowerStr = s;
    for(size_t i = 0; i < lowerStr.length(); ++i) lowerStr[i] = tolower(lowerStr[i]);
    return lowerStr;
}

struct Date {
    int day, month, year;
    
    string toString() const { 
        return intToString(day) + "/" + intToString(month) + "/" + intToString(year); 
    }
    
    static int daysBetween(Date d1, Date d2) {
        long t1 = d1.year * 365 + d1.month * 30 + d1.day;
        long t2 = d2.year * 365 + d2.month * 30 + d2.day;
        return (int)(t2 - t1);
    }
    
    static Date getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        Date d; d.day = ltm->tm_mday; d.month = 1 + ltm->tm_mon; d.year = 1900 + ltm->tm_year;
        return d;
    }
};

#endif // UTILS_CPP
