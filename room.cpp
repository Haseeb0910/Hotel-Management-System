#include"room.h"
#include<iostream>
#include<iomanip>
#include<sstream>
using namespace std;

Room::Room() : room_no(0) , type("") , availability(true) , price_per_night(0.0) {};

Room::Room(int num , string t , double price) : room_no(num) , type(t) , availability(true) , price_per_night(price) {};

void Room::display_RoomInfo() const
{
    cout << "Room number: " << room_no << endl << "Type: " << type << endl << "Price Per Night: " <<fixed<<setprecision(2)<< price_per_night << endl << (availability ? "Yes" : "No") << endl;
}

void Room::setAvailibility(bool status) 
{
    availability = status;
}

bool Room::getAvailibility() const
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

string Room::toCSV() const
{
    return to_string(room_no) + "," + type + "," + to_string(price_per_night) + "," + to_string(availability ? 1 : 0);
}

void Room::fromCSV(const string& line)
{
    stringstream ss(line);
    string temp;

    getline(ss , temp , ',');
    room_no = stoi(temp);
    getline(ss , type , ',');
    getline(ss , temp , ',');
    price_per_night = stof(temp);
    getline(ss , temp , ',');
    availability = (temp == "1");
}