#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "01_Movie.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// BookingService
// Knows   : the Cinema, the catalogue of Movies/Shows, and every Booking
//           made so far (needed to look bookings up by id for cancellation).
// Does    : orchestrates the WHOLE booking flow end to end — this is the
//           only class allowed to coordinate Show, ShowSeat, PriceCalculator,
//           Payment, Booking and TicketPrinter together.
// Must NOT: format console menus itself (MainMenu's job) or know payment
//           internals (Payment's job) or know how a ticket looks
//           (TicketPrinter's job).
// ---------------------------------------------------------------------------
class BookingService {
private:
    Cinema *cinema;
    vector<Movie*> movies;
    vector<Show*> shows;
    vector<Booking*> bookings;
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;

    static int nextBookingId;              // Static Members: shared by all BookingService use.

public:
    BookingService(Cinema *cinema) : cinema(cinema) {}   // constructor #1

    // Compile-time polymorphism: overloaded constructor that also seeds movies/shows.
    BookingService(Cinema *cinema, vector<Movie*> movies, vector<Show*> shows)
        : cinema(cinema), movies(movies), shows(shows) {}

    void addMovie(Movie *m) { movies.push_back(m); }
    void addShow(Show *s) { shows.push_back(s); }

    const vector<Movie*>& getMovies() const { return movies; }

    vector<Show*> getShowsForMovie(Movie *m) const {
        vector<Show*> result;
        for (Show *s : shows) {
            if (s->getMovie() == m) result.push_back(s);
        }
        return result;
    }

    // ---- F1: list movies -------------------------------------------------
    void listMovies() const {
        if (movies.empty()) { cout << "  No movies currently playing.\n"; return; }
        for (size_t i = 0; i < movies.size(); i++) {
            cout << "  [" << (i + 1) << "] " << movies[i]->getTitle()
                 << "\t" << movies[i]->getLanguage()
                 << "\t" << movies[i]->getDuration() << " min\n";
        }
    }

    // ---- F2: list shows for a movie --------------------------------------
    void listShowsForMovie(Movie *m) const {
        vector<Show*> ms = getShowsForMovie(m);
        if (ms.empty()) { cout << "  No shows scheduled for this movie.\n"; return; }
        for (size_t i = 0; i < ms.size(); i++) {
            cout << "  [" << (i + 1) << "] Screen-" << ms[i]->getScreen()->getScreenNumber()
                 << "   " << ms[i]->getStartTime() << "\n";
        }
    }

    // ---- F4/FR4: book seats (rejects if ANY seat already booked) --------
    // Returns nullptr on rejection; caller checks for that.
    Booking* bookSeats(Show *show, const vector<string> &seatNumbers, Customer *customer) {
        vector<ShowSeat*> chosen;

        // First pass: validate every seat exists AND is available.
        // "if ANY selected seat is already BOOKED, the WHOLE booking is
        // rejected and no seat changes state" -> we never call book() until
        // every single seat has passed validation.
        for (const string &num : seatNumbers) {
            ShowSeat *ss = show->findShowSeat(num);
            if (ss == nullptr) {
                cout << "  Seat " << num << " does not exist on this screen.\n";
                return nullptr;
            }
            if (!ss->isAvailable()) {
                cout << "  Seat " << num << " is already BOOKED. Booking rejected.\n";
                return nullptr;
            }
            chosen.push_back(ss);
        }

        // Second pass: only now do we mutate state, since every seat passed.
        for (ShowSeat *ss : chosen) ss->book();

        double total = priceCalculator.calculateTotal(chosen);

        string id = "BK" + to_string(1000 + nextBookingId);
        nextBookingId++;

        Booking *booking = new Booking(id, show, chosen, customer, total);
        bookings.push_back(booking);
        return booking;
    }

    // ---- F6/FR6: pay for a booking ---------------------------------------
    bool makePayment(Booking *booking, PaymentType method, const string &accountDetail) {
        Payment *payment = nullptr;
        if (method == PaymentType::UPI) payment = new UpiPayment(accountDetail);
        else if (method == PaymentType::CARD) payment = new CardPayment(accountDetail);
        else payment = new CashPayment();

        bool success = payment->pay(booking->getTotalAmount());   // Runtime polymorphism
        booking->setPayment(payment);

        if (success) {
            booking->confirm();
        } else {
            // Payment failed: release every seat back to AVAILABLE, mark booking FAILED.
            for (ShowSeat *ss : booking->getSeats()) ss->release();
            booking->fail();
        }
        return success;
    }

    // ---- F7: print ticket --------------------------------------------------
    void printTicket(Booking &booking) const { ticketPrinter.printTicket(booking); }

    // ---- F8/FR8: cancel a booking ------------------------------------------
    bool cancelBooking(const string &bookingId) {
        for (Booking *b : bookings) {
            if (b->getBookingId() == bookingId) {
                if (b->getStatus() != BookingStatus::CONFIRMED) {
                    cout << "  Booking " << bookingId << " is not an active CONFIRMED booking.\n";
                    return false;
                }
                for (ShowSeat *ss : b->getSeats()) ss->release();
                b->cancel();
                cout << "  Booking " << bookingId << " cancelled. Seats released.\n";
                return true;
            }
        }
        cout << "  No such booking id: " << bookingId << "\n";
        return false;
    }

    vector<Booking*>& getAllBookings() { return bookings; }
};

int BookingService::nextBookingId = 1;     // static member definition
