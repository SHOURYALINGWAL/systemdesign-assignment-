#pragma once
#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// Movie
// Knows   : its own title, language, duration.
// Does    : nothing but hand that data back out (pure data holder).
// Must NOT: know about shows, screens, seats, bookings or pricing.
// ---------------------------------------------------------------------------
class Movie {
private:                                   // Encapsulation: state is private,
    string title;                          // only reachable through getters.
    string language;
    int durationMin;

public:
    Movie(string title, string language, int durationMin)
        : title(title), language(language), durationMin(durationMin) {}

    string getTitle() const { return title; }
    string getLanguage() const { return language; }
    int getDuration() const { return durationMin; }
};
