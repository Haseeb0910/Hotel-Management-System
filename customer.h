#ifndef CUSTOMER_H
#define CUSTOMER_H

#include<string>
using namespace std;

class Customer
{
    private:
    string name;
    string cnic;
    string phone_no;
    public:
    Customer();
    Customer(string n , string c , string p);
    void display_CustomerInfo() const;

    string getName() const;
    string getCnic() const;
    string getPhone() const;

    string toCSV() const;
    void fromCSV(const string& line);
};

#endif