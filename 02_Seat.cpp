#pragma once
#include <string>
using namespace std;

// SeatType is a property of a physical seat (not a class of its own —
// see Item B, noun-verb analysis: it has no behaviour, only a value).
enum class SeatType { SILVER, GOLD, PLATINUM };

// Constants instead of magic numbers (clean-code checklist).
const double SILVER_PRICE = 150.0;
const double GOLD_PRICE = 250.0;
const double PLATINUM_PRICE = 400.0;

string seatTypeToString(SeatType t) {
    if (t == SeatType::SILVER) return "SILVER";
    if (t == SeatType::GOLD) return "GOLD";
    return "PLATINUM";
}

double priceForSeatType(SeatType t) {
    if (t == SeatType::SILVER) return SILVER_PRICE;
    if (t == SeatType::GOLD) return GOLD_PRICE;
    return PLATINUM_PRICE;
}

// ---------------------------------------------------------------------------
// Seat
// Knows   : its own seat number ("A1") and its type (SILVER/GOLD/PLATINUM).
// Does     : hands that data back out.
// Must NOT: know whether it is booked for any particular show — that is
//           ShowSeat's job (a seat is physical hardware; booking status is
//           per-show data, see the "Why ShowSeat" note in the assignment).
// ---------------------------------------------------------------------------
class Seat {
private:
    string seatNumber;
    SeatType type;

public:
    Seat(string seatNumber, SeatType type)
        : seatNumber(seatNumber), type(type) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
};
