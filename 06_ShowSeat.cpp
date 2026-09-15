#pragma once
#include "02_Seat.cpp"
using namespace std;

enum class SeatStatus { AVAILABLE, BOOKED };

// ---------------------------------------------------------------------------
// ShowSeat
// Knows   : which physical Seat it represents, and its AVAILABLE/BOOKED
//           status FOR ONE SPECIFIC SHOW (see assignment note: seat A1 may
//           be booked for the 6 PM show and free for the 9 PM show — status
//           belongs to the show, not to the physical chair).
// Does    : flips its own status via book()/release(), with a guard so a
//           seat can never be double-booked from inside the class itself.
// Must NOT: know which Show or Booking it belongs to, and must NOT
//           calculate price (that's PriceCalculator's job).
// ---------------------------------------------------------------------------
class ShowSeat {
private:
    Seat *seat;                            // Aggregation-like use: points at a Seat owned by Screen.
    SeatStatus status;

public:
    ShowSeat(Seat *seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

    void book() { status = SeatStatus::BOOKED; }      // caller must check isAvailable() first
    void release() { status = SeatStatus::AVAILABLE; }

    Seat* getSeat() const { return seat; }
    SeatStatus getStatus() const { return status; }
};
