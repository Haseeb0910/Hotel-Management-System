#ifndef HOTEL_H
#define HOTEL_H

#include <vector>
#include "room.h"
#include "customer.h"
#include "booking.h"
using namespace std;

class Hotel {
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Booking> bookings;
    int nextBookingID;

public:
    Hotel();

    void addRoom(const Room& room);
    void addCustomer(const Customer& customer);
    void bookRoom(int roomNumber, const string& customerCNIC, const string& checkIn, const string& checkOut);

    void showAllRooms() const;
    void showAllCustomers() const;
    void showAllBookings() const;

    void loadSampleData();
};

#endif
