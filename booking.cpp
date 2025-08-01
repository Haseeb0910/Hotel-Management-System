#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
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

void Booking::write_to_file(ofstream& out) const
{
   out.write(reinterpret_cast<const char*>(&bookingID) , sizeof(bookingID));
   out.write(reinterpret_cast<const char*>(&roomNumber) , sizeof(roomNumber));

   size_t len = customerCNIC.length();
   out.write(reinterpret_cast<const char*>(&len) , sizeof(len));
   out.write(customerCNIC.c_str() , len);

   len = checkInDate.length();
   out.write(reinterpret_cast<const char*>(&len) , sizeof(len));
   out.write(checkInDate.c_str() , len);

   len = checkOutDate.length();
   out.write(reinterpret_cast<const char*>(&len) , sizeof(len));
   out.write(checkOutDate.c_str() , len);
}

void Booking::read_from_file(ifstream& in)
{
    in.read(reinterpret_cast<char*>(&bookingID) , sizeof(bookingID));
    in.read(reinterpret_cast<char*>(&roomNumber) , sizeof(roomNumber));

    size_t len;

    in.read(reinterpret_cast<char*>(&len) , sizeof(len));
    unique_ptr<char[]> buffer1(new char[len + 1]);
    in.read(buffer1.get() , len);
    buffer1[len] = '\0';
    customerCNIC = string(buffer1.get());

    in.read(reinterpret_cast<char*>(&len) , sizeof(len));
    unique_ptr<char[]> buffer2(new char[len + 1]);
    in.read(buffer2.get() , len);
    buffer2[len] = '\0';
    checkInDate = string(buffer2.get());

    in.read(reinterpret_cast<char*>(&len) , sizeof(len));
    unique_ptr<char[]> buffer3(new char[len + 1]);
    in.read(buffer3.get() , len);
    buffer3[len] = '\0';
    checkOutDate = string(buffer3.get());
}