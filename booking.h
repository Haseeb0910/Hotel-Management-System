#ifndef BOOKING_H
#define BOOKING_H

#include <string>
using namespace std;

class Booking {
private:
    int bookingID;
    int roomNumber;
    string customerCNIC;
    string checkInDate;
    string checkOutDate;

public:
    Booking();
    Booking(int id, int room, string cnic, string in, string out);

    void displayBookingInfo() const;

    int getBookingID() const;
    int getRoomNumber() const;
    string getCustomerCNIC() const;
    string getCheckInDate() const;
    string getCheckOutDate() const;

    string toCSV() const;
    void fromCSV(const string& line);
};

#endif
