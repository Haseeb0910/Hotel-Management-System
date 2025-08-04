#include "Date.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>

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
    iss >> day >> sep >> month >> sep >> year;
    validateDate();
}

void Date::validateDate() const
{
    if (year < 1900 || year > 2100)
    {
        throw invalid_argument("Year must be between 1900 and 2100");
    }
    if (month < 1 || month > 12)
    {
        throw invalid_argument("Month must be between 1 and 12");
    }

    int maxDays = daysInMonth();
    if (day < 1 || day > maxDays)
    {
        throw invalid_argument("Day is invalid for this month");
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
    while (days > 0)
    {
        int remaining = result.daysInMonth() - result.day + 1;
        if (days >= remaining)
        {
            days -= remaining;
            result.day = 1;
            if (++result.month > 12)
            {
                result.month = 1;
                result.year++;
            }
        }
        else
        {
            result.day += days;
            days = 0;
        }
    }
    return result;
}

int Date::daysBetween(const Date &other) const
{
    const Date &from = *this < other ? *this : other;
    const Date &to = *this < other ? other : *this;

    int days = 0;
    Date current(from);
    while (current < to)
    {
        current = current.addDays(1);
        days++;
    }
    return days;
}

Date Date::maxDate()
{
    return Date(31, 12, 2100); 
}

Date Date::currentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
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

    // Validate the read date
    try
    {
        validateDate();
    }
    catch (const invalid_argument &e)
    {
        // Reset to default date if invalid
        day = 1;
        month = 1;
        year = 2000;
        throw; // Re-throw to handle corruption
    }
}