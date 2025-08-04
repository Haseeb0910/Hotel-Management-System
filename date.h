#ifndef DATE_H
#define DATE_H

#include <string>
#include <stdexcept>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;

    void validateDate() const;

public:
    Date();
    Date(int d, int m, int y);
    explicit Date(const string& dateStr); // Format: "DD/MM/YYYY"
    
    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    // Comparison operators
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    
    // Utility functions
    string toString() const;
    bool isLeapYear() const;
    int daysInMonth() const;
    
    // Date arithmetic
    Date addDays(int days) const;
    int daysBetween(const Date& other) const;

    static Date maxDate();
    static Date currentDate();

    void write_to_file(ofstream& out) const;
    void read_from_file(ifstream& in);
};

#endif