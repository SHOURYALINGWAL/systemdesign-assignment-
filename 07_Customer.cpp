#pragma once
#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// Customer
// Knows   : its own name and phone number.
// Does    : nothing beyond exposing that data.
// Must NOT: know about bookings, shows or payments — Customer -> BookingService
//           is a plain Association (Customer uses the service; it doesn't own it).
// ---------------------------------------------------------------------------
class Customer {
private:
    string name;
    string phone;

public:
    Customer(string name, string phone) : name(name), phone(phone) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
};
