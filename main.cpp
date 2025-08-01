#include <iostream>
#include <limits>
#include <cstdlib> // For system("CLS") on Windows
#include "hotel.h"
#include "room.h"
#include "customer.h"

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void showMenu() {
    cout << "\n___ HOTEL MANAGEMENT SYSTEM ___" << endl;
    cout << "1. Add Room" << endl;
    cout << "2. Add Customer" << endl;
    cout << "3. Book Room" << endl;
    cout << "4. Show All Rooms" << endl;
    cout << "5. Show All Customers" << endl;
    cout << "6. Show All Bookings" << endl;
    cout << "7. Save Data" << endl;
    cout << "8. Load Data" << endl;
    cout << "9. Cancel Booking" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value)
            break;
        else {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore(); // Clear leftover newline
    return value;
}

double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value)
            break;
        else {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore();
    return value;
}

int main() {
    Hotel myHotel;
    int choice;

    myHotel.loadData(); // Load at startup

    do {
        clearScreen();
        showMenu();
        choice = getValidatedInt("");

        switch (choice) {
        case 1: {
            int num = getValidatedInt("Enter Room Number: ");
            string type;
            cout << "Enter Room Type: ";
            getline(cin, type);
            double price = getValidatedDouble("Enter Price per Night: ");
            myHotel.addRoom(Room(num, type, price));
            break;
        }
        case 2: {
            string name, cnic, phone;
            cout << "Enter Customer Name: ";
            getline(cin, name);
            cout << "Enter Customer CNIC: ";
            getline(cin, cnic);
            cout << "Enter Customer Phone No: ";
            getline(cin, phone);
            myHotel.addCustomer(Customer(name, cnic, phone));
            break;
        }
        case 3: {
            int roomNum = getValidatedInt("Enter Room Number: ");
            string cnic, inDate, outDate;
            cout << "Enter Customer CNIC: ";
            getline(cin, cnic);
            cout << "Enter Check-in Date (DD/MM/YYYY): ";
            getline(cin, inDate);
            cout << "Enter Check-out Date (DD/MM/YYYY): ";
            getline(cin, outDate);
            myHotel.bookRoom(roomNum, cnic, inDate, outDate);
            break;
        }
        case 4:
            myHotel.showAllRooms();
            break;
        case 5:
            myHotel.showAllCustomers();
            break;
        case 6:
            myHotel.showAllBookings();
            break;
        case 7:
            myHotel.saveData();
            cout << "Data saved successfully!\n";
            break;
        case 8:
            myHotel.loadData();
            cout << "Data loaded successfully!\n";
            break;
        case 9: {
            int bookingID = getValidatedInt("Enter Booking ID to cancel: ");
            myHotel.cancelBooking(bookingID);
            break;
        }
        case 0:
            cout << "Saving data before exiting...\n";
            myHotel.saveData();
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }

    } while (choice != 0);

    return 0;
}
