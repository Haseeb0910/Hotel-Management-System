#ifndef HOTEL_H
#define HOTEL_H

#include <vector>
#include "room.h"
#include "customer.h"
#include "booking.h"
using namespace std;

class Hotel
{
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Booking> bookings;
    int nextBookingID;

    void validateDataConsistency(); 

public:
    Hotel();

    void addRoom(int num, RoomType type, double price);
    void addCustomer(const Customer &customer);
    void bookRoom(int roomNumber, const string &customerCNIC,const Date &checkIn, const Date &checkOut);

    bool hasBookingOverlap(int roomNumber, const Date &checkIn, const Date &checkOut) const;

    void showAllRooms() const;
    void showAllCustomers() const;
    void showAllBookings() const;

    bool roomExists(int roomNum) const;
    bool customerExists(const string &cnic) const;
    bool isRoomAvailable(int roomNum) const;

    void cancelBooking(int bookingID);

    vector<Room> findAvailableRooms(RoomType type) const;
    vector<Customer> findCustomersByName(const string& name) const;

    void saveData();
    void loadData();
};

#endif
