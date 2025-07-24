#include <iostream>
#include "hotel.h"

using namespace std;

Hotel::Hotel() : nextBookingID(1) {}

void Hotel::addRoom(const Room &room)
{
    rooms.push_back(room);
}

void Hotel::addCustomer(const Customer &customer)
{
    customers.push_back(customer);
}

void Hotel::bookRoom(int roomNumber, const string &customerCNIC, const string &checkIn, const string &checkOut)
{
    Booking new_Booking(nextBookingID++, roomNumber, customerCNIC, checkIn, checkOut);
    bookings.push_back(new_Booking);
    cout << "Room Booked Successfully" << endl;
}

void Hotel::showAllRooms() const
{
    if (rooms.size() == 0)
    {
        cout << endl
             << "No Rooms" << endl
             << endl;
    }
    else
    {
        cout << endl
             << "___All Rooms___" << endl;
        for (int i = 0; i < rooms.size(); i++)
        {
            rooms[i].display_RoomInfo();
        }
    }
}

void Hotel::showAllCustomers() const
{
    if (customers.size() == 0)
    {
        cout << endl
             << "No Customers" << endl
             << endl;
    }
    else
    {
        cout << endl
             << "___All Customers___" << endl;
        for (int i = 0; i < customers.size(); i++)
        {
            customers[i].display_CustomerInfo();
        }
    }
}

void Hotel::showAllBookings() const
{
    if (bookings.size() == 0)
    {
        cout << endl
             << "No Bookings" << endl
             << endl;
    }
    else
    {
        cout << endl
             << "___All Bookings___" << endl;
        for (int i = 0; i < bookings.size(); i++)
        {
            bookings[i].displayBookingInfo();
        }
    }
}

void Hotel::loadSampleData()
{
    addRoom(Room(101, "Single", 5000.0));
    addRoom(Room(102, "Double", 8000.0));
    addCustomer(Customer("Ali Raza", "35202-1234567-1", "03001234567"));
}