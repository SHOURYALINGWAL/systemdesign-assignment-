#pragma once
#include <vector>
#include "06_ShowSeat.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// PriceCalculator
// Knows   : nothing (stateless service).
// Does    : turns a list of ShowSeats into a total amount, using each
//           underlying Seat's type-based price.
// Must NOT: know about bookings, payments, or shows — pure calculation only.
// ---------------------------------------------------------------------------
class PriceCalculator {
public:
    double calculateTotal(const vector<ShowSeat*> &seats) const {
        double total = 0.0;
        for (ShowSeat *ss : seats) {
            total += priceForSeatType(ss->getSeat()->getType());
        }
        return total;
    }
};
