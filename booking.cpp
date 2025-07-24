#include <iostream>
#include <sstream>
#include "booking.h"

using namespace std;

Booking::Booking() : bookingID(0), roomNumber(0), customerCNIC(""), checkInDate(""), checkOutDate("") {}

Booking::Booking(int id, int room, string cnic, string in, string out) : bookingID(id), roomNumber(room), customerCNIC(cnic), checkInDate(in), checkOutDate(out) {}

void Booking::displayBookingInfo() const
{
    cout << "Booking ID: " << bookingID << endl
         << "Room Number: " << roomNumber << endl
         << "Customer CNIC: " << customerCNIC << endl
         << "Check-in Date: " << checkInDate << endl
         << "Check-out Date: " << checkOutDate << endl;
}

int Booking::getBookingID() const { return bookingID; }
int Booking::getRoomNumber() const { return roomNumber; }
string Booking::getCustomerCNIC() const { return customerCNIC; }
string Booking::getCheckInDate() const { return checkInDate; }
string Booking::getCheckOutDate() const { return checkOutDate; }

string Booking::toCSV() const
{
    return to_string(bookingID) + "," + to_string(roomNumber) + "," + customerCNIC + "," + checkInDate + "," + checkOutDate;
}

void Booking::fromCSV(const string& line)
{
    stringstream ss(line);
    string temp;

    getline(ss , temp , ',');
    bookingID = stoi(temp);
    getline(ss , temp , ',');
    roomNumber = stoi(temp);
    getline(ss , customerCNIC , ',');
    getline(ss , checkInDate , ',');
    getline(ss , checkOutDate , ',');
}