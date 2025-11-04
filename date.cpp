#include "Date.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <stdexcept>

using namespace std;

Date::Date() : day(1), month(1), year(2000) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y)
{
    validateDate();
}

Date::Date(const string &dateStr)
{
    char sep;
    istringstream iss(dateStr);
    if (!(iss >> day >> sep >> month >> sep >> year) || sep != '/')
    {
        throw invalid_argument("Invalid date format. Use DD/MM/YYYY");
    }
    validateDate();
}

void Date::validateDate() const
{
    if (year < 1800 || year > 2500)
    {
        throw invalid_argument("Year must be between 1800 and 2500");
    }
    if (month < 1 || month > 12)
    {
        throw invalid_argument("Month must be between 1 and 12");
    }
    if (day < 1 || day > daysInMonth())
    {
        throw invalid_argument("Day " + to_string(day) + " is invalid for month " + to_string(month) + " in year " + to_string(year));
    }
}

bool Date::isLeapYear() const
{
    return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
}

int Date::daysInMonth() const
{
    switch (month)
    {
    case 2:
        return isLeapYear() ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        return 31;
    }
}

bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date &other) const
{
    return *this < other || *this == other;
}

bool Date::operator>(const Date &other) const
{
    return !(*this <= other);
}

bool Date::operator>=(const Date &other) const
{
    return !(*this < other);
}

bool Date::operator==(const Date &other) const
{
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator!=(const Date &other) const
{
    return !(*this == other);
}

string Date::toString() const
{
    ostringstream oss;
    oss << setfill('0') << setw(2) << day << "/"
        << setw(2) << month << "/" << setw(4) << year;
    return oss.str();
}

Date Date::addDays(int days) const
{
    Date result(*this);
    int remaining_days = days;
    while (remaining_days != 0)
    {
        int step = remaining_days > 0 ? 1 : -1;
        result.day += step;
        remaining_days -= step;

        if (result.day > result.daysInMonth())
        {
            result.day = 1;
            if (++result.month > 12)
            {
                result.month = 1;
                result.year++;
            }
        }
        else if (result.day < 1)
        {
            if (--result.month < 1)
            {
                result.month = 12;
                result.year--;
            }
            result.day = result.daysInMonth();
        }
        result.validateDate();
    }
    return result;
}

int Date::daysBetween(const Date &other) const
{
    auto toJulianDay = [](int day, int month, int year)
    {
        int a = (14 - month) / 12;
        int y = year + 4800 - a;
        int m = month + 12 * a - 3;
        return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    };

    int jd1 = toJulianDay(day, month, year);
    int jd2 = toJulianDay(other.day, other.month, other.year);
    return abs(jd2 - jd1);
}

Date Date::maxDate()
{
    return Date(31, 12, 2500);
}

Date Date::currentDate()
{
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto *tm = localtime(&time_t);
    return Date(tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

void Date::write_to_file(ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&day), sizeof(day));
    out.write(reinterpret_cast<const char *>(&month), sizeof(month));
    out.write(reinterpret_cast<const char *>(&year), sizeof(year));
}

void Date::read_from_file(ifstream &in)
{
    in.read(reinterpret_cast<char *>(&day), sizeof(day));
    in.read(reinterpret_cast<char *>(&month), sizeof(month));
    in.read(reinterpret_cast<char *>(&year), sizeof(year));
    if (!in)
    {
        day = 1;
        month = 1;
        year = 2000;
        throw runtime_error("Failed to read date from file");
    }
    try
    {
        validateDate();
    }
    catch (const invalid_argument &e)
    {
        day = 1;
        month = 1;
        year = 2000;
        throw runtime_error("Corrupted date in file: " + string(e.what()));
    }
}