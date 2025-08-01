#ifndef ROOM_H
#define ROOM_H

#include<string>
using namespace std;

class Room
{
    private:
    int room_no;
    string type;
    bool availability;
    double price_per_night;
    public:
    Room();
    Room(int num , string t , double price);
    void display_RoomInfo() const;
    void setAvailability(bool status);
    bool getAvailability() const;
    int getRoomNumber() const;
    void setPrice(double p);
    double getPrice() const;

    string toCSV() const;
    void fromCSV(const string& line);

    void write_to_file(ofstream& out) const;
    void read_from_file(ifstream& in);
};

#endif