#include "room.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

Room::Room() : room_no(0), type(RoomType::STANDARD), availability(true), price_per_night(0.0) {}

Room::Room(int num, RoomType t, double price) : room_no(num), type(t), availability(true), price_per_night(price) {}

void Room::display_RoomInfo() const
{
    cout << "Room number: " << room_no << endl
         << "Type: " << getTypeString() << endl
         << "Price Per Night: " << fixed << setprecision(2) << price_per_night << endl
         << "Available: " << (availability ? "Yes" : "No") << endl;
}

void Room::setAvailability(bool status)
{
    availability = status;
}

bool Room::getAvailability() const
{
    return availability;
}

int Room::getRoomNumber() const
{
    return room_no;
}

void Room::setPrice(double p)
{
    price_per_night = p;
}

double Room::getPrice() const
{
    return price_per_night;
}

string Room::getTypeString() const
{
    switch (type)
    {
    case RoomType::STANDARD:
        return "Standard";
    case RoomType::DELUXE:
        return "Deluxe";
    case RoomType::SUITE:
        return "Suite";
    case RoomType::FAMILY:
        return "Family";
    default:
        return "Unknown";
    }
}

RoomType Room::stringToType(const string &typeStr)
{
    string upperStr = typeStr;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);

    if (upperStr == "STANDARD")
        return RoomType::STANDARD;
    if (upperStr == "DELUXE")
        return RoomType::DELUXE;
    if (upperStr == "SUITE")
        return RoomType::SUITE;
    if (upperStr == "FAMILY")
        return RoomType::FAMILY;

    return RoomType::STANDARD; // Default value
}

string Room::toCSV() const
{
    return to_string(room_no) + "," + getTypeString() + "," + to_string(price_per_night) + "," + to_string(availability ? 1 : 0);
}

void Room::fromCSV(const string &line)
{
    stringstream ss(line);
    string temp;

    getline(ss, temp, ',');
    room_no = stoi(temp);
    getline(ss, temp, ',');
    type = stringToType(temp);
    getline(ss, temp, ',');
    price_per_night = stof(temp);
    getline(ss, temp, ',');
    availability = (temp == "1");
}

void Room::write_to_file(ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&room_no), sizeof(room_no));
    RoomType enumValue = type;
    out.write(reinterpret_cast<const char *>(&enumValue), sizeof(enumValue));
    out.write(reinterpret_cast<const char *>(&price_per_night), sizeof(price_per_night));
    out.write(reinterpret_cast<const char *>(&availability), sizeof(availability));
}

void Room::read_from_file(ifstream &in)
{
    in.read(reinterpret_cast<char *>(&room_no), sizeof(room_no));
    RoomType enumValue;
    in.read(reinterpret_cast<char *>(&enumValue), sizeof(enumValue));
    type = enumValue;
    in.read(reinterpret_cast<char *>(&price_per_night), sizeof(price_per_night));
    in.read(reinterpret_cast<char *>(&availability), sizeof(availability));
}