#pragma once
#include <string>
#include <vector>
#include "03_Screen.cpp"
using namespace std;

// ---------------------------------------------------------------------------
// Cinema
// Knows   : its name and the fixed set of Screens it contains.
// Does    : owns its screens (Composition: Cinema 1 *-- 1..* Screen).
// Must NOT: know about movies, shows or bookings — a Cinema is just the
//           physical building with auditoriums in it.
// ---------------------------------------------------------------------------
class Cinema {
private:
    string name;
    vector<Screen> screens;

public:
    Cinema(string name) : name(name) {}

    void addScreen(const Screen &screen) { screens.push_back(screen); }

    string getName() const { return name; }
    vector<Screen>& getScreens() { return screens; }

    Screen* findScreen(int screenNumber) {
        for (auto &s : screens) {
            if (s.getScreenNumber() == screenNumber) return &s;
        }
        return nullptr;
    }
};
