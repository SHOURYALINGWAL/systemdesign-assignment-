#pragma once
#include <vector>
#include "02_Seat.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// Screen
// Knows   : its screen number and the fixed set of physical Seats inside it.
// Does    : builds/owns its seats (Composition: Screen 1 *-- 1..* Seat).
// Must NOT: know about movies, shows, or booking status — a Screen is just
//           hardware, reused by every show scheduled on it.
// ---------------------------------------------------------------------------
class Screen {
private:
    int screenNumber;
    vector<Seat> seats;                    // Composition: seats live and die with the Screen.

public:
    Screen(int screenNumber) : screenNumber(screenNumber) {}

    // Compile-time polymorphism (overloading): two ways to add seats.
    void addSeat(const Seat &seat) { seats.push_back(seat); }

    void addSeatRange(char rowLetter, int count, SeatType type) {
        for (int i = 1; i <= count; i++) {
            string seatNumber = string(1, rowLetter) + to_string(i);
            seats.push_back(Seat(seatNumber, type));
        }
    }

    int getScreenNumber() const { return screenNumber; }
    const vector<Seat>& getSeats() const { return seats; }
};
