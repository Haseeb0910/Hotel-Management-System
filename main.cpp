#include <iostream>
#include <limits>
#include <cstdlib> // For system("CLS") on Windows
#include <functional>
#include "hotel.h"
#include "room.h"
#include "customer.h"

using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void showMenu()
{
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

int getValidatedInt(const string &prompt, function<bool(int)> validator = nullptr)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            if (!validator || validator(value))
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Invalid input! Please try again." << endl;
        }
        else
        {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double getValidatedDouble(const string &prompt, function<bool(double)> validator = nullptr)
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            if (!validator || validator(value))
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Invalid input! Please enter a positive value." << endl;
        }
        else
        {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main()
{
    Hotel myHotel;
    int choice;

    try {
        myHotel.loadData(); // Load at startup
        cout << "Data loaded successfully!" << endl;
    } catch (const exception& e) {
        cout << "Note: Could not load previous data: " << e.what() << endl;
        cout << "Starting with fresh data..." << endl;
    }

    do
    {
        clearScreen();
        showMenu();
        
        try {
            choice = getValidatedInt("");
        } catch (...) {
            cout << "Error reading input. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int num = getValidatedInt("Enter Room Number: ");

            // Display available room types
            cout << "\nAvailable Room Types:\n"
                 << "1. Standard\n"
                 << "2. Deluxe\n"
                 << "3. Suite\n"
                 << "4. Family\n";

            // Get and validate room type
            int typeChoice = getValidatedInt("\nSelect Room Type (1-4): ", [](int val)
                                             { return val >= 1 && val <= 4; });

            // Convert to enum
            RoomType type;
            switch (typeChoice)
            {
            case 1:
                type = RoomType::STANDARD;
                break;
            case 2:
                type = RoomType::DELUXE;
                break;
            case 3:
                type = RoomType::SUITE;
                break;
            case 4:
                type = RoomType::FAMILY;
                break;
            default:
                type = RoomType::STANDARD; // fallback
            }

            // Get and validate price
            double price = getValidatedDouble("\nEnter Price per Night: $", [](double val)
                                              {
                                                  return val > 0; // Must be positive
                                              });

            // Add the room
            myHotel.addRoom(num, type, price);
            break;
        }
        case 2:
        {
            string name, cnic, phone;
            cout << "Enter Customer Name: ";
            getline(cin, name);
            cout << "Enter Customer CNIC: ";
            getline(cin, cnic);
            cout << "Enter Customer Phone No: ";
            getline(cin, phone);
            
            if (name.empty() || cnic.empty() || phone.empty()) {
                cout << "Error: All fields are required!" << endl;
            } else {
                myHotel.addCustomer(Customer(name, cnic, phone));
            }
            break;
        }
        case 3:
        {
            int roomNum = getValidatedInt("Enter Room Number: ");
            string cnic;
            cout << "Enter Customer CNIC: ";
            getline(cin, cnic);

            string inDateStr, outDateStr;
            Date currentDate = Date::currentDate();

            while (true)
            {
                cout << "Enter Check-in Date (DD/MM/YYYY): ";
                getline(cin, inDateStr);
                cout << "Enter Check-out Date (DD/MM/YYYY): ";
                getline(cin, outDateStr);

                try
                {
                    Date checkIn(inDateStr);
                    Date checkOut(outDateStr);

                    // Validate dates are in future and check-out after check-in
                    if (checkIn < currentDate)
                    {
                        throw invalid_argument("Check-in date must be in the future");
                    }
                    if (checkOut <= checkIn)
                    {
                        throw invalid_argument("Check-out date must be after check-in");
                    }

                    myHotel.bookRoom(roomNum, cnic, checkIn, checkOut);
                    break;
                }
                catch (const invalid_argument &e)
                {
                    cout << "Error: " << e.what() << ". Please try again.\n";
                }
            }
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
            try {
                myHotel.saveData();
                cout << "Data saved successfully!\n";
            } catch (const exception& e) {
                cout << "Error saving data: " << e.what() << endl;
            }
            break;
        case 8:
            try {
                myHotel.loadData();
                cout << "Data loaded successfully!\n";
            } catch (const exception& e) {
                cout << "Error loading data: " << e.what() << endl;
            }
            break;
        case 9:
        {
            int bookingID = getValidatedInt("Enter Booking ID to cancel: ");
            myHotel.cancelBooking(bookingID);
            break;
        }
        case 0:
            cout << "Saving data before exiting...\n";
            try {
                myHotel.saveData();
                cout << "Data saved successfully!\n";
            } catch (const exception& e) {
                cout << "Warning: " << e.what() << endl;
            }
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }

        if (choice != 0)
        {
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }

    } while (choice != 0);

    return 0;
}