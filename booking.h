#ifndef BOOKING_H
#define BOOKING_H

#include "Date.h"
#include <string>
using namespace std;

enum class PaymentStatus
{
    PENDING,
    PAID,
    CANCELLED,
    REFUNDED
};

class Booking
{
private:
    int bookingID;
    int roomNumber;
    string customerCNIC;
    Date checkInDate;
    Date checkOutDate;

    PaymentStatus paymentStatus;
    double amountPaid;
    double nightlyRate;

public:
    Booking();
    Booking(int id, int room, string cnic, Date in, Date out);

    void displayBookingInfo() const;

    int getBookingID() const { return bookingID; }
    int getRoomNumber() const { return roomNumber; }
    string getCustomerCNIC() const { return customerCNIC; }
    const Date &getCheckInDate() const { return checkInDate; }
    const Date &getCheckOutDate() const { return checkOutDate; }

    void setNightlyRate(double rate) { nightlyRate = rate; }
    double calculateTotalAmount() const;
    void makePayment(double amount);
    PaymentStatus getPaymentStatus() const { return paymentStatus; }
    void refundPayment(double amount);

    string toCSV() const;
    void fromCSV(const string &line);

    void write_to_file(ofstream &out) const;
    void read_from_file(ifstream &in);
};

#endif
