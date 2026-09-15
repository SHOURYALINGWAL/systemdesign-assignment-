#pragma once
#include <iostream>
#include "08_Booking.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// TicketPrinter
// Knows   : nothing (stateless service).
// Does    : formats and prints a ticket for a CONFIRMED booking — PRINTING
//           ONLY, no side effects on booking/seat state (clean-code rule:
//           a display function must not mutate anything).
// Must NOT: change booking status, seat status, or any other object's state.
// ---------------------------------------------------------------------------
class TicketPrinter {
public:
    void printTicket(Booking &booking) const {
        if (booking.getStatus() != BookingStatus::CONFIRMED) {
            cout << "  Cannot print ticket: booking is not CONFIRMED.\n";
            return;
        }

        cout << "\n  ================ TICKET ================\n";
        cout << "  Booking ID : " << booking.getBookingId() << "\n";
        cout << "  Movie      : " << booking.getShow()->getMovie()->getTitle() << "\n";
        cout << "  Screen     : Screen-" << booking.getShow()->getScreen()->getScreenNumber()
             << "   " << booking.getShow()->getStartTime() << "\n";
        cout << "  Seats      : ";
        for (size_t i = 0; i < booking.getSeats().size(); i++) {
            cout << booking.getSeats()[i]->getSeat()->getSeatNumber();
            if (i + 1 < booking.getSeats().size()) cout << ", ";
        }
        cout << "\n";
        cout << "  Amount     : Rs." << booking.getTotalAmount()
             << "        Status: " << bookingStatusToString(booking.getStatus()) << "\n";
        cout << "  ========================================\n";
    }
};
