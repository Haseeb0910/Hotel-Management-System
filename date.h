#ifndef DATE_H
#define DATE_H

#include <string>
#include <fstream>
using namespace std;

class Date
{
private:
    int day, month, year;

public:
    Date();
    Date(int d, int m, int y);
    Date(const string &dateStr);

    bool operator<(const Date &other) const;
    bool operator<=(const Date &other) const;
    bool operator>(const Date &other) const;
    bool operator>=(const Date &other) const;
    bool operator==(const Date &other) const;
    bool operator!=(const Date &other) const;

    void validateDate() const;
    bool isLeapYear() const;
    int daysInMonth() const;

    string toString() const;
    Date addDays(int days) const;
    int daysBetween(const Date &other) const;

    static Date maxDate();
    static Date currentDate();

    void write_to_file(ofstream &out) const;
    void read_from_file(ifstream &in);
};

#endif