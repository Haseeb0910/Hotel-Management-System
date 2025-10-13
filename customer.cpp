#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
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

bool Customer::isValidCNIC(const string &cnic)
{
   if (cnic.length() != 13)
      return false;

   return all_of(cnic.begin(), cnic.end(), ::isdigit);
}

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

void Customer::write_to_file(ofstream &out) const
{
   size_t len1 = name.length();
   out.write(reinterpret_cast<const char *>(&len1), sizeof(len1));
   out.write(name.c_str(), len1);
   size_t len2 = cnic.length();
   out.write(reinterpret_cast<const char *>(&len2), sizeof(len2));
   out.write(cnic.c_str(), len2);
   size_t len3 = phone_no.length();
   out.write(reinterpret_cast<const char *>(&len3), sizeof(len3));
   out.write(phone_no.c_str(), len3);
}

void Customer::read_from_file(ifstream &in)
{
   size_t len;
   in.read(reinterpret_cast<char *>(&len), sizeof(len));
   unique_ptr<char[]> buffer1(new char[len + 1]);
   in.read(buffer1.get() , len);
   buffer1[len] = '\0';
   name = string(buffer1.get());
   in.read(reinterpret_cast<char *>(&len), sizeof(len));
   unique_ptr<char[]> buffer2(new char[len + 1]);
   in.read(buffer2.get(), len);
   buffer2[len] = '\0';
   cnic = string(buffer2.get());
   in.read(reinterpret_cast<char *>(&len), sizeof(len));
   unique_ptr<char[]> buffer3(new char[len + 1]);
   in.read(buffer3.get(), len);
   buffer3[len] = '\0';
   phone_no = string(buffer3.get());
}