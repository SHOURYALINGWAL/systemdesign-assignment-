// =============================================================================
// main.cpp — the console menu and input reading (the "Cinema/main menu" role
// from Step C). Holds NO domain data itself; every action is delegated to
// BookingService. This keeps the menu free to change (wording, flow order)
// without ever touching booking/pricing/payment logic.
// One-class-per-file / no-header-files rule is honoured via #pragma once +
// direct .cpp includes, forming a single translation unit.
// =============================================================================
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"
using namespace std;

// splits "A1,B2" into {"A1","B2"}, trimming stray spaces (helper, not a class:
// pure input-parsing utility used only by this file).
vector<string> splitSeats(const string &line) {
    vector<string> result;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        if (start != string::npos) result.push_back(token.substr(start, end - start + 1));
    }
    return result;
}

// Reads a menu choice safely; on non-numeric input, returns -1 instead of
// crashing (NFR3: invalid input must never crash the program).
int readMenuChoice() {
    string line;
    getline(cin, line);
    try {
        return stoi(line);
    } catch (...) {
        return -1;
    }
}

void seedDemoData(Cinema &cinema, BookingService &service,
                   vector<Movie*> &movieStore, vector<Show*> &showStore) {
    Screen screen1(1);
    screen1.addSeatRange('A', 4, SeatType::SILVER);
    screen1.addSeatRange('B', 3, SeatType::GOLD);
    screen1.addSeatRange('C', 2, SeatType::PLATINUM);
    cinema.addScreen(screen1);

    Screen screen2(2);
    screen2.addSeatRange('A', 4, SeatType::SILVER);
    screen2.addSeatRange('B', 3, SeatType::GOLD);
    cinema.addScreen(screen2);

    movieStore.push_back(new Movie("Mirzapur", "Hindi", 200));
    movieStore.push_back(new Movie("Blade Runner", "English", 180));

    Screen *s1 = cinema.findScreen(1);
    Screen *s2 = cinema.findScreen(2);

    showStore.push_back(new Show(1, "06:00 PM", movieStore[0], s1));
    showStore.push_back(new Show(2, "09:00 PM", movieStore[0], s2));
    showStore.push_back(new Show(3, "07:30 PM", movieStore[1], s2));

    for (Movie *m : movieStore) service.addMovie(m);
    for (Show *s : showStore) service.addShow(s);
}

int main() {
    Cinema cinema("PVR CityMall");
    vector<Movie*> movieStore;
    vector<Show*> showStore;
    BookingService service(&cinema);
    seedDemoData(cinema, service, movieStore, showStore);

    cout << "===== MOVIE TICKET BOOKING =====\n";

    while (true) {
        cout << "\n1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\nChoose: ";
        int choice = readMenuChoice();

        if (choice == 0) {
            cout << "Goodbye!\n";
            break;

        } else if (choice == 1) {
            // F1
            cout << "\n";
            service.listMovies();

        } else if (choice == 2) {
            // Full booking flow: F1 -> F2 -> F3 -> F4 -> F5 -> F6 -> F7
            cout << "\n";
            service.listMovies();
            const vector<Movie*> &movies = service.getMovies();
            if (movies.empty()) continue;

            cout << "Choose movie: ";
            int mChoice = readMenuChoice();
            if (mChoice < 1 || mChoice > (int)movies.size()) {
                cout << "  Invalid movie choice.\n"; continue;
            }
            Movie *movie = movies[mChoice - 1];

            vector<Show*> shows = service.getShowsForMovie(movie);
            service.listShowsForMovie(movie);
            if (shows.empty()) continue;

            cout << "Choose show: ";
            int sChoice = readMenuChoice();
            if (sChoice < 1 || sChoice > (int)shows.size()) {
                cout << "  Invalid show choice.\n"; continue;
            }
            Show *show = shows[sChoice - 1];

            // F3
            show->printSeatLayout();

            cout << "\nSeats (e.g. A1,B2): ";
            string seatLine;
            getline(cin, seatLine);
            vector<string> seatNumbers = splitSeats(seatLine);
            if (seatNumbers.empty()) { cout << "  No seats entered.\n"; continue; }

            cout << "Customer name: ";
            string name; getline(cin, name);
            Customer customer(name, "");

            // F4 + FR5 (pricing shown before payment)
            Booking *booking = service.bookSeats(show, seatNumbers, &customer);
            if (booking == nullptr) continue;   // rejected: message already printed

            cout << "  ";
            for (ShowSeat *ss : booking->getSeats()) {
                cout << ss->getSeat()->getSeatNumber() << " "
                     << seatTypeToString(ss->getSeat()->getType())
                     << " Rs." << priceForSeatType(ss->getSeat()->getType()) << "   ";
            }
            cout << "\n  TOTAL          Rs." << booking->getTotalAmount() << "\n";

            // F6
            cout << "\nPay by: 1.UPI  2.Card  3.Cash > ";
            int payChoice = readMenuChoice();
            PaymentType method;
            string detail = "";
            if (payChoice == 1) { method = PaymentType::UPI; cout << "  UPI ID: "; getline(cin, detail); }
            else if (payChoice == 2) { method = PaymentType::CARD; cout << "  Card number: "; getline(cin, detail); }
            else if (payChoice == 3) { method = PaymentType::CASH; }
            else { cout << "  Invalid payment choice. Booking left unpaid (seats will remain booked).\n"; continue; }

            bool paid = service.makePayment(booking, method, detail);

            // F7
            if (paid) {
                service.printTicket(*booking);
            } else {
                cout << "  Booking " << booking->getBookingId()
                     << " FAILED. Seats released back to AVAILABLE.\n";
            }

        } else if (choice == 3) {
            // F8
            cout << "Booking ID to cancel: ";
            string id; getline(cin, id);
            service.cancelBooking(id);

        } else if (choice == 4) {
            auto &all = service.getAllBookings();
            if (all.empty()) { cout << "  No bookings yet.\n"; continue; }
            for (Booking *b : all) {
                cout << "  " << b->getBookingId() << "  "
                     << b->getShow()->getMovie()->getTitle() << "  "
                     << bookingStatusToString(b->getStatus()) << "\n";
            }

        } else {
            cout << "  Invalid menu choice. Please choose 0-4.\n";
        }
    }

    return 0;
}
