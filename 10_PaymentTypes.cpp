#pragma once
#include <iostream>
#include <string>
#include "09_Payment.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// UpiPayment / CardPayment / CashPayment
// Inheritance: each IS-A Payment (extends the Payment contract).
// Runtime polymorphism: BookingService always calls through a Payment*,
// and the correct pay() below runs depending on the ACTUAL object type.
//
// Demo convention: entering OTP/PIN "0000" simulates a failed payment,
// so the "payment fails" edge case (#2) can be triggered on demand.
// Cash payment always succeeds (money is handed over in person).
// ---------------------------------------------------------------------------
class UpiPayment : public Payment {
private:
    string upiId;

public:
    UpiPayment(string upiId) : upiId(upiId) {}

    bool pay(double amount) override {
        this->amount = amount;
        cout << "  Enter UPI OTP (use 0000 to simulate failure): ";
        string otp; cin >> otp;
        bool success = (otp != "0000");
        cout << "  [UPI] Rs." << amount
             << (success ? " paid successfully\n" : " payment FAILED (wrong OTP)\n");
        return success;
    }
};

class CardPayment : public Payment {
private:
    string cardNumber;

public:
    CardPayment(string cardNumber) : cardNumber(cardNumber) {}

    bool pay(double amount) override {
        this->amount = amount;
        cout << "  Enter Card PIN (use 0000 to simulate failure): ";
        string pin; cin >> pin;
        bool success = (pin != "0000");
        cout << "  [CARD] Rs." << amount
             << (success ? " paid successfully\n" : " payment FAILED (wrong PIN)\n");
        return success;
    }
};

class CashPayment : public Payment {
public:
    CashPayment() {}

    bool pay(double amount) override {
        this->amount = amount;
        cout << "  [CASH] Rs." << amount << " collected successfully\n";
        return true;
    }
};
