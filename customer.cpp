#include <iostream>
#include <sstream>
#include "customer.h"
using namespace std;

Customer::Customer() : name(""), cnic(""), phone_no("") {}

Customer::Customer(string n, string c, string p) : name(n), cnic(c), phone_no(p) {}

void Customer::display_CustomerInfo() const
{
   cout << "Customer Name: " << name << endl
        << "Customer CNIC: " << cnic << endl
        << "Customer Phone No: " << phone_no << endl;
}

string Customer::getName() const { return name; }
string Customer::getCnic() const { return cnic; }
string Customer::getPhone() const { return phone_no; }

string Customer::toCSV() const
{
   return (name + "," + cnic + "," + phone_no);
}

void Customer::fromCSV(const string &line)
{
   stringstream ss(line);
   getline(ss, name, ',');
   getline(ss, cnic, ',');
   getline(ss, phone_no, ',');
}