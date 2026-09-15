#pragma once
#include <string>
#include <vector>
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
#include "09_Payment.cpp"
using namespace std;

enum class BookingStatus { PENDING, CONFIRMED, CANCELLED, FAILED };

string bookingStatusToString(BookingStatus s) {
    if (s == BookingStatus::PENDING) return "PENDING";
    if (s == BookingStatus::CONFIRMED) return "CONFIRMED";
    if (s == BookingStatus::CANCELLED) return "CANCELLED";
    return "FAILED";
}

// ---------------------------------------------------------------------------
// Booking
// Knows   : its own id, which Show, which ShowSeats, which Customer, the
//           total amount and its own status.
// Does    : manages its OWN status transitions (confirm/cancel/fail).
// Must NOT: print tickets (that's TicketPrinter's job — Single Responsibility:
//           "who asks for a layout change vs a pricing change" would hit two
//           different classes if Booking printed itself) and must NOT
//           calculate its own total (that's PriceCalculator's job).
// ---------------------------------------------------------------------------
class Booking {
private:
    string bookingId;
    Show *show;                            // Aggregation
    vector<ShowSeat*> seats;               // Aggregation: seats outlive/are owned by Show
    Customer *customer;                    // Association
    double totalAmount;
    BookingStatus status;
    Payment *payment;                      // Association (nullptr until payment attempted)

public:
    Booking(string bookingId, Show *show, vector<ShowSeat*> seats,
            Customer *customer, double totalAmount)
        : bookingId(bookingId), show(show), seats(seats), customer(customer),
          totalAmount(totalAmount), status(BookingStatus::PENDING), payment(nullptr) {}

    void confirm() { status = BookingStatus::CONFIRMED; }

    void fail() { status = BookingStatus::FAILED; }

    void cancel() { status = BookingStatus::CANCELLED; }

    void setPayment(Payment *p) { payment = p; }

    string getBookingId() const { return bookingId; }
    Show* getShow() const { return show; }
    vector<ShowSeat*>& getSeats() { return seats; }
    Customer* getCustomer() const { return customer; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }
};
