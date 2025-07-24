#include <iostream>
#include "hotel.h"
#include "room.h"
#include "customer.h"

using namespace std;

void showMenu()
{
    cout << "___HOTEL MANAGEMENT SYSTEM___" << endl;
    cout << "1. Add Room" << endl;
    cout << "2. Add Customer" << endl;
    cout << "3. Book Room" << endl;
    cout << "4. Show All Rooms" << endl;
    cout << "5. Show All Customers" << endl;
    cout << "6. Show All Bookings" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int main()
{
    Hotel myHotel;
    int choice;

    do
    {
        showMenu();
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            int num;
            string type;
            double price;
            cout << "Enter Room No: ";
            cin >> num;
            cout << "Enter Room Type: ";
            cin >> type;
            cout << "Enter Price per night: ";
            cin >> price;
            myHotel.addRoom(Room(num, type, price));
            break;
        }
        case 2:
        {
            string name, cnic, phone_no;
            cout << "Enter Customer Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Customer CNIC: ";
            getline(cin, cnic);
            cout << "Enter Customer Phone No: ";
            getline(cin, phone_no);
            myHotel.addCustomer(Customer(name, cnic, phone_no));
            break;
        }
        case 3:
        {
            int roomNum;
            string cnic, inDate, outDate;
            cout << "Enter room number: ";
            cin >> roomNum;
            cout << "Enter customer CNIC: ";
            cin.ignore();
            getline(cin, cnic);
            cout << "Enter check-in date: ";
            getline(cin, inDate);
            cout << "Enter check-out date: ";
            getline(cin, outDate);
            myHotel.bookRoom(roomNum, cnic, inDate, outDate);
            break;
        }
        case 4: myHotel.showAllRooms(); break;
        case 5: myHotel.showAllCustomers(); break;
        case 6: myHotel.showAllBookings(); break;
        case 0: cout << "Exiting Program" << endl; break;
        default:
            cout << "Invalid Choice. Try Again";
            break;
        }
    } while (choice != 0);

    return 0;
}