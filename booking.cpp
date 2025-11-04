#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "booking.h"

using namespace std;

Booking::Booking() : bookingID(0), roomNumber(0), customerCNIC(""),
                     checkInDate(), checkOutDate(), // Remove empty strings
                     paymentStatus(PaymentStatus::PENDING), amountPaid(0.0), nightlyRate(0.0)
{
}

Booking::Booking(int id, int room, string cnic, Date in, Date out) : bookingID(id), roomNumber(room), customerCNIC(cnic),
                                                                     checkInDate(in), checkOutDate(out), // Use the Date objects directly
                                                                     paymentStatus(PaymentStatus::PENDING), amountPaid(0.0), nightlyRate(0.0)
{
}

void Booking::displayBookingInfo() const
{
    cout << "Booking ID: " << bookingID << endl
         << "Room Number: " << roomNumber << endl
         << "Customer CNIC: " << customerCNIC << endl
         << "Check-in Date: " << checkInDate.toString() << endl
         << "Check-out Date: " << checkOutDate.toString() << endl
         << "Payment Status: ";
    switch (paymentStatus)
    {
    case PaymentStatus::PAID:
        cout << "Paid";
        break;
    case PaymentStatus::PENDING:
        cout << "Pending";
        break;
    case PaymentStatus::CANCELLED:
        cout << "Cancelled";
        break;
    case PaymentStatus::REFUNDED:
        cout << "Refunded";
        break;
    }
    cout << endl
         << "Amount Paid: " << amountPaid << endl
         << "Total Amount: " << calculateTotalAmount() << endl;
}

double Booking::calculateTotalAmount() const
{
    int nights = checkInDate.daysBetween(checkOutDate);
    return nights * nightlyRate;
}

void Booking::makePayment(double amount)
{
    if (checkInDate.daysBetween(checkOutDate) <= 0)
    {
        throw invalid_argument("Invalid booking duration");
    }
    amountPaid += amount;
    if (amountPaid >= calculateTotalAmount())
    {
        paymentStatus = PaymentStatus::PAID;
    }
}

void Booking::refundPayment(double amount)
{
    if (amount <= 0 || amount > amountPaid)
    {
        throw invalid_argument("Invalid refund amount");
    }
    amountPaid -= amount;
    if (amountPaid == 0)
    {
        paymentStatus = PaymentStatus::REFUNDED;
    }
}

string Booking::toCSV() const
{
    return to_string(bookingID) + "," + to_string(roomNumber) + "," + customerCNIC + "," + checkInDate.toString() + "," + checkOutDate.toString() + "," + to_string(static_cast<int>(paymentStatus)) + "," + to_string(amountPaid) + "," + to_string(nightlyRate);
}

void Booking::fromCSV(const string &line)
{
    stringstream ss(line);
    string temp;

    getline(ss, temp, ',');
    bookingID = stoi(temp);
    getline(ss, temp, ',');
    roomNumber = stoi(temp);
    getline(ss, customerCNIC, ',');
    getline(ss, temp, ',');
    checkInDate = Date(temp);
    getline(ss, temp, ',');
    checkOutDate = Date(temp);
    getline(ss, temp, ',');
    paymentStatus = static_cast<PaymentStatus>(stoi(temp));
    getline(ss, temp, ',');
    amountPaid = stod(temp);
    getline(ss, temp, ',');
    nightlyRate = stod(temp);
}

void Booking::write_to_file(ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&bookingID), sizeof(bookingID));
    out.write(reinterpret_cast<const char *>(&roomNumber), sizeof(roomNumber));

    size_t len = customerCNIC.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(customerCNIC.c_str(), len);

    checkInDate.write_to_file(out);
    checkOutDate.write_to_file(out);

    out.write(reinterpret_cast<const char *>(&paymentStatus), sizeof(paymentStatus));
    out.write(reinterpret_cast<const char *>(&amountPaid), sizeof(amountPaid));
    out.write(reinterpret_cast<const char *>(&nightlyRate), sizeof(nightlyRate));
}

void Booking::read_from_file(ifstream &in)
{
    in.read(reinterpret_cast<char *>(&bookingID), sizeof(bookingID));
    in.read(reinterpret_cast<char *>(&roomNumber), sizeof(roomNumber));

    size_t len;

    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    unique_ptr<char[]> buffer1(new char[len + 1]);
    in.read(buffer1.get(), len);
    buffer1[len] = '\0';
    customerCNIC = string(buffer1.get());

    checkInDate.read_from_file(in);
    checkOutDate.read_from_file(in);

    in.read(reinterpret_cast<char *>(&paymentStatus), sizeof(paymentStatus));
    in.read(reinterpret_cast<char *>(&amountPaid), sizeof(amountPaid));
    in.read(reinterpret_cast<char *>(&nightlyRate), sizeof(nightlyRate));
}