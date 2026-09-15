#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// Show
// Knows   : which Movie (borrowed, aggregation) plays on which Screen
//           (borrowed, aggregation) at what startTime, and owns one
//           ShowSeat per physical seat of that screen (composition).
// Does    : builds its ShowSeat list from the Screen's seats, and can print
//           its own seat layout (FR3).
// Must NOT: calculate prices, take payments, or create bookings — Show only
//           reports seat state, it never mutates business/money logic.
// ---------------------------------------------------------------------------
class Show {
private:
    int showId;
    string startTime;
    Movie *movie;                          // Aggregation: Show does not own the Movie's lifetime.
    Screen *screen;                        // Aggregation: Show does not own the Screen's lifetime.
    vector<ShowSeat> showSeats;            // Composition: these die with this Show.

public:
    Show(int showId, string startTime, Movie *movie, Screen *screen)
        : showId(showId), startTime(startTime), movie(movie), screen(screen) {
        for (const Seat &s : screen->getSeats()) {
            // const_cast is safe here: ShowSeat only reads seat number/type,
            // never mutates the underlying Seat.
            showSeats.push_back(ShowSeat(const_cast<Seat*>(&s)));
        }
    }

    int getShowId() const { return showId; }
    string getStartTime() const { return startTime; }
    Movie* getMovie() const { return movie; }
    Screen* getScreen() const { return screen; }
    vector<ShowSeat>& getShowSeats() { return showSeats; }

    ShowSeat* findShowSeat(const string &seatNumber) {
        for (auto &ss : showSeats) {
            if (ss.getSeat()->getSeatNumber() == seatNumber) return &ss;
        }
        return nullptr;
    }

    void printSeatLayout() const {
        cout << "\n  SCREEN-" << screen->getScreenNumber() << "  " << startTime
             << "  |  " << movie->getTitle() << "\n";

        for (SeatType t : {SeatType::SILVER, SeatType::GOLD, SeatType::PLATINUM}) {
            cout << "  " << seatTypeToString(t);
            for (int i = seatTypeToString(t).size(); i < 8; i++) cout << " ";
            bool any = false;
            for (const auto &ss : showSeats) {
                if (ss.getSeat()->getType() == t) {
                    any = true;
                    cout << ss.getSeat()->getSeatNumber()
                         << (ss.isAvailable() ? "[ ] " : "[X] ");
                }
            }
            if (!any) cout << "(none)";
            cout << "\n";
        }
        cout << "  ( [ ] = available   [X] = booked )\n";
    }
};
