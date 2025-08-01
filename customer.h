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

    static bool isValidCNIC(const string& cnic);

    string toCSV() const;
    void fromCSV(const string& line);

    void write_to_file(ofstream& out) const;
    void read_from_file(ifstream& in);
};

#endif