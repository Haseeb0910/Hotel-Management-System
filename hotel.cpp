#include <iostream>
#include <fstream>
#include <algorithm>
#include "hotel.h"

using namespace std;

void Hotel::validateDataConsistency()
{
    // Check for orphaned bookings (no matching customer or room)
    for (const auto &booking : bookings)
    {
        if (!customerExists(booking.getCustomerCNIC()))
        {
            throw runtime_error("Data corruption: Booking " + to_string(booking.getBookingID()) +
                                " references non-existent customer");
        }

        if (!roomExists(booking.getRoomNumber()))
        {
            throw runtime_error("Data corruption: Booking " + to_string(booking.getBookingID()) +
                                " references non-existent room");
        }
    }

    // Check room availability flags match booking status
    for (const auto &room : rooms)
    {
        bool shouldBeAvailable = !hasBookingOverlap(room.getRoomNumber(), Date(), Date::maxDate());
        if (room.getAvailability() != shouldBeAvailable)
        {
            throw runtime_error("Data corruption: Room " + to_string(room.getRoomNumber()) +
                                " availability flag is incorrect");
        }
    }
}

Hotel::Hotel() : nextBookingID(1) {}

void Hotel::addRoom(int num, RoomType type, double price)
{
    // Check if room number already exists
    for (const Room &r : rooms)
    {
        if (r.getRoomNumber() == num)
        {
            cout << "Room number already exists. Cannot add again." << endl;
            return;
        }
    }

    // Add the new room
    rooms.emplace_back(num, type, price);
    saveData();
    cout << "Room added successfully!" << endl;
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

void Hotel::bookRoom(int roomNumber, const string &customerCNIC, const Date &checkIn, const Date &checkOut)
{
    // Basic validation
    if (checkIn >= checkOut)
    {
        cout << "Check-out date must be after check-in date.\n";
        return;
    }

    // Check if customer exists
    if (!customerExists(customerCNIC))
    {
        cout << "Customer with CNIC " << customerCNIC << " not found." << endl;
        return;
    }

    // Check room exists
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

    // Check for booking conflicts using the corrected logic
    if (hasBookingOverlap(roomNumber, checkIn, checkOut))
    {
        cout << "Room is already booked for those dates." << endl;
        return;
    }

    // Create new booking
    bookings.emplace_back(nextBookingID++, roomNumber, customerCNIC, checkIn, checkOut);
    bookings.back().setNightlyRate(roomPtr->getPrice());
    roomPtr->setAvailability(false);
    cout << "Room booked successfully. Booking ID: " << (nextBookingID - 1) << endl;
    saveData();
}

void Hotel::updateRoomAvailability(int roomNumber)
{
    auto roomIt = find_if(rooms.begin(), rooms.end(),
                          [roomNumber](const Room &r)
                          { return r.getRoomNumber() == roomNumber; });

    if (roomIt != rooms.end())
    {
        Date today = Date::currentDate();
        bool hasActiveBooking = any_of(bookings.begin(), bookings.end(),
                                       [roomNumber, today](const Booking &b)
                                       {
                                           return b.getRoomNumber() == roomNumber &&
                                                  b.getCheckInDate() <= today &&
                                                  b.getCheckOutDate() > today;
                                       });
        roomIt->setAvailability(!hasActiveBooking);
    }
}

bool Hotel::hasBookingOverlap(int roomNumber, const Date &checkIn, const Date &checkOut) const
{
    for (const auto &booking : bookings)
    {
        if (booking.getRoomNumber() == roomNumber)
        {
            // Check if the new booking overlaps with existing booking
            bool overlap = !(checkOut <= booking.getCheckInDate() || checkIn >= booking.getCheckOutDate());
            if (overlap)
            {
                return true;
            }
        }
    }
    return false;
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
        {
            // Check if there are any overlapping bookings
            return !hasBookingOverlap(roomNum, Date(), Date::maxDate());
        }
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

vector<Room> Hotel::findAvailableRooms(RoomType type) const
{
    vector<Room> result;
    copy_if(rooms.begin(), rooms.end(), back_inserter(result),
            [type](const Room &r)
            {
                return r.getAvailability() && r.getType() == type;
            });
    return result;
}

vector<Customer> Hotel::findCustomersByName(const string &name) const
{
    vector<Customer> result;
    copy_if(customers.begin(), customers.end(), back_inserter(result),
            [&name](const Customer &c)
            {
                return c.getName().find(name) != string::npos;
            });
    return result;
}

void Hotel::saveData()
{
    auto saveVector = [](const auto &vec, const string &filename)
    {
        ofstream out(filename, ios::binary);
        if (!out)
        {
            throw runtime_error("Failed to open " + filename + " for writing");
        }
        size_t count = vec.size();
        out.write(reinterpret_cast<char *>(&count), sizeof(count));
        for (const auto &item : vec)
        {
            item.write_to_file(out);
        }
        if (!out)
        {
            throw runtime_error("Error writing to " + filename);
        }
    };

    try
    {
        saveVector(rooms, "Rooms.dat");
        saveVector(customers, "Customers.dat");
        saveVector(bookings, "Bookings.dat");
    }
    catch (const exception &e)
    {
        cerr << "Error saving data: " << e.what() << endl;
    }
}

void Hotel::loadData()
{
    // Clear existing data
    rooms.clear();
    customers.clear();
    bookings.clear();
    nextBookingID = 1;

    // Simple file existence check - if files don't exist, start fresh (which is fine)
    ifstream roomIn("Rooms.dat", ios::binary);
    if (!roomIn) {
        cout << "No previous room data found. Starting fresh." << endl;
        return;
    }

    // If we reach here, files exist but we'll be cautious
    cout << "Previous data found but appears corrupted. Starting with fresh data." << endl;
    
    // Don't actually load the corrupted data
    rooms.clear();
    customers.clear();
    bookings.clear();
    nextBookingID = 1;
}