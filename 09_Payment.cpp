#pragma once

enum class PaymentType { UPI, CARD, CASH };

// ---------------------------------------------------------------------------
// Payment (ABSTRACT)
// Abstraction: defines the payment CONTRACT only — pay(amount) — with no
// implementation of its own. Every concrete payment method must honour
// this exact contract (Liskov Substitution: any Payment* can be paid
// through with no extra setup call).
// ---------------------------------------------------------------------------
class Payment {
protected:
    double amount;

public:
    Payment() : amount(0.0) {}
    virtual bool pay(double amount) = 0;   // pure virtual -> Abstraction
    virtual ~Payment() {}                  // virtual destructor: safe polymorphic deletion
};
