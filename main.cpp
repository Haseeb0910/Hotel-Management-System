#include<iostream>
#include "customer.h"
using namespace std;

int main() {
    Customer c1("Ali Raza", "35202-1234567-1", "03001234567");
    c1.display_CustomerInfo();

    string csv = c1.toCSV();
    cout << "\nCSV: " << csv << endl;

    Customer c2;
    c2.fromCSV(csv);
    cout << "\nCustomer from CSV:\n";
    c2.display_CustomerInfo();

    return 0;
}
