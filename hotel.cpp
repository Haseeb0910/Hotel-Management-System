#include <iostream>
#include <fstream>
#include <algorithm>
#include "hotel.h"

using namespace std;

Hotel::Hotel() : nextBookingID(1) {}

void Hotel::addRoom(const Room &room)
{
    for (Room &r : rooms)
    {
        if (r.getRoomNumber() == room.getRoomNumber())
        {
            cout << "Room number already exists. Cannot add again." << endl;
            return;
        }
    }
    rooms.push_back(room);
    saveData();
    cout << "Room added successfully" << endl;
}

void Hotel::addCustomer(const Customer &customer)
{
    if (!Customer::isValidCNIC(customer.getCnic()))
    {
        cout << "Invalid CNIC format! Must be 13 digits." << endl;
        return;
    }
    for (Customer &c : customers)
    {
        if (c.getCnic() == customer.getCnic())
        {
            cout << "Customer with this CNIC already exists." << endl;
            return;
        }
    }
    customers.push_back(customer);
    saveData();
    cout << "Customer added successfully" << endl;
}

void Hotel::bookRoom(int roomNumber, const string &customerCNIC, const string &checkIn, const string &checkOut)
{
    if (!Customer::isValidCNIC(customerCNIC))
    {
        cout << "Invalid CNIC format! Must be 13 digits." << endl;
        return;
    }
    // Check room exists and is available
    Room *roomPtr = nullptr;
    for (Room &r : rooms)
    {
        if (r.getRoomNumber() == roomNumber)
        {
            roomPtr = &r;
            break;
        }
    }
    if (!roomPtr)
    {
        cout << "Room not found." << endl;
        return;
    }
    if (!roomPtr->getAvailability())
    {
        cout << "Room is not available for booking." << endl;
        return;
    }

    // Check customer exists
    bool found = false;
    for (Customer &c : customers)
    {
        if (c.getCnic() == customerCNIC)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Customer not found. Please register customer first." << endl;
        return;
    }

    // Date format
    if (checkIn.length() != 10 || checkIn[2] != '/' || checkIn[5] != '/' ||
        checkOut.length() != 10 || checkOut[2] != '/' || checkOut[5] != '/')
    {
        cout << "Please enter date in format DD/MM/YYYY.\n";
        return;
    }

    // Add booking
    bookings.emplace_back(nextBookingID++, roomNumber, customerCNIC, checkIn, checkOut);
    roomPtr->setAvailability(false);
    saveData();
    cout << "Room booked successfully.\n";
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

bool Hotel::roomExists(int roomNum) const
{
    return any_of(rooms.begin(), rooms.end(), [roomNum](const Room &r)
                  { return r.getRoomNumber() == roomNum; });
}

bool Hotel::customerExists(const string &cnic) const
{
    return any_of(customers.begin(), customers.end(), [cnic](const Customer &c)
                  { return c.getCnic() == cnic; });
}

bool Hotel::isRoomAvailable(int roomNum) const
{
    for (const Room &r : rooms)
    {
        if (r.getRoomNumber() == roomNum)
            return r.getAvailability();
    }
    return false;
}

void Hotel::cancelBooking(int bookingID)
{
    if (bookings.empty())
    {
        cout << "No Booking available to cancel" << endl;
        return;
    }

    bool found = false;

    for (auto i = bookings.begin(); i != bookings.end(); i++)
    {
        if (i->getBookingID() == bookingID)
        {
            for (Room &room : rooms)
            {
                if (i->getRoomNumber() == room.getRoomNumber())
                {
                    room.setAvailability(true);
                    break;
                }
            }
            bookings.erase(i);
            saveData();
            cout << "Booking cancelled successfully." << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Booking ID not found." << endl;
    saveData();
}

void Hotel::saveData()
{
    // save Rooms
    ofstream room_out("Rooms.dat", ios::binary);
    if (!room_out)
    {
        cerr << "Failed to open Rooms.dat for writing\n";
        return;
    }
    size_t roomCount = rooms.size();
    room_out.write(reinterpret_cast<char *>(&roomCount), sizeof(roomCount));
    for (int i = 0; i < roomCount; i++)
        rooms[i].write_to_file(room_out);
    room_out.close();

    // save Customers
    ofstream customer_out("Customers.dat", ios::binary);
    size_t customerCount = customers.size();
    customer_out.write(reinterpret_cast<char *>(&customerCount), sizeof(customerCount));
    for (int i = 0; i < customerCount; i++)
        customers[i].write_to_file(customer_out);
    customer_out.close();

    // save Bookings
    ofstream booking_out("Bookings.dat", ios::binary);
    size_t bookingCount = bookings.size();
    booking_out.write(reinterpret_cast<char *>(&bookingCount), sizeof(bookingCount));
    for (int i = 0; i < bookingCount; i++)
        bookings[i].write_to_file(booking_out);
    booking_out.close();
}

void Hotel::loadData()
{
    // load rooms
    ifstream room_in("Rooms.dat", ios::binary);
    if (room_in)
    {
        size_t roomCount;
        room_in.read(reinterpret_cast<char *>(&roomCount), sizeof(roomCount));
        rooms.clear();
        for (int i = 0; i < roomCount; i++)
        {
            Room r;
            r.read_from_file(room_in);
            rooms.push_back(r);
        }
        room_in.close();
    }

    // load customers
    ifstream customer_in("Customers.dat", ios::binary);
    if (customer_in)
    {
        size_t customerCount;
        customer_in.read(reinterpret_cast<char *>(&customerCount), sizeof(customerCount));
        customers.clear();
        for (int i = 0; i < customerCount; i++)
        {
            Customer c;
            c.read_from_file(customer_in);
            customers.push_back(c);
        }
        customer_in.close();
    }

    // load bookings
    ifstream booking_in("Bookings.dat", ios::binary);
    if (booking_in)
    {
        size_t bookingCount;
        booking_in.read(reinterpret_cast<char *>(&bookingCount), sizeof(bookingCount));
        bookings.clear();
        for (int i = 0; i < bookingCount; i++)
        {
            Booking b;
            b.read_from_file(booking_in);
            bookings.push_back(b);
        }
        if (!bookings.empty())
        {
            nextBookingID = max_element(bookings.begin(), bookings.end(),
                                        [](const Booking &a, const Booking &b)
                                        {
                                            return a.getBookingID() < b.getBookingID();
                                        })
                                ->getBookingID() +
                            1;
        }
        else
        {
            nextBookingID = 1;
        }
        booking_in.close();
    }
}
