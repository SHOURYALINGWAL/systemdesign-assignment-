# 🎬 CineBook — Movie Ticket Booking System

A menu-driven **C++17** console application that simulates a single-cinema movie ticket booking system (think PVR / INOX) — browse movies and shows, view live seat availability, book seats, pay, print a ticket, and cancel a booking.

Built as an OOP design exercise: the emphasis is on clean class boundaries, correct UML relationships, and demonstrable **SOLID** design — not just a working console app.

---

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Architecture](#architecture)
  - [Class Diagram](#class-diagram)
  - [Sequence Diagram](#sequence-diagram-book-1-seat--pay-by-upi)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Design Principles](#design-principles)
- [Edge Cases Handled](#edge-cases-handled)
- [Requirements Reference](#requirements-reference)
- [Documentation](#documentation)
- [License](#license)

---

## Features

| # | Feature |
|---|---|
| F1 | List all movies currently playing |
| F2 | For a chosen movie, list its shows (screen + start time) |
| F3 | For a chosen show, display the live seat layout — `AVAILABLE` / `BOOKED` |
| F4 | Book one or more seats for a show (atomically rejects if **any** seat is already booked) |
| F5 | Price the booking by seat type — `SILVER ₹150` · `GOLD ₹250` · `PLATINUM ₹400` |
| F6 | Pay by **UPI**, **Card**, or **Cash** — a failed payment never confirms the booking |
| F7 | Print a ticket — booking id, movie, screen, time, seats, total amount |
| F8 | Cancel a booking — seats are released back to `AVAILABLE` |

## Demo

```
===== MOVIE TICKET BOOKING =====

1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit
Choose: 2

  [1] Mirzapur      Hindi   200 min
  [2] Blade Runner  English 180 min
Choose movie: 1
  [1] Screen-1   06:00 PM
  [2] Screen-2   09:00 PM
Choose show: 1

  SCREEN-1  06:00 PM  |  Mirzapur
  SILVER  A1[ ] A2[ ] A3[ ] A4[ ]
  GOLD    B1[ ] B2[ ] B3[ ]
  PLATINUM C1[ ] C2[ ]
  ( [ ] = available   [X] = booked )

Seats (e.g. A1,B2): A1,B2
Customer name: Shourya_Lingwal
  A1 SILVER Rs.150   B2 GOLD Rs.250
  TOTAL          Rs.400

Pay by: 1.UPI  2.Card  3.Cash > 1
  UPI ID: Shourya_Lingwal@upi
  Enter UPI OTP (use 0000 to simulate failure): 1234
  [UPI] Rs.400 paid successfully

  ================ TICKET ================
  Booking ID : BK1001
  Movie      : Mirzapur
  Screen     : Screen-1   06:00 PM
  Seats      : A1, B2
  Amount     : Rs.400        Status: CONFIRMED
  ========================================
```

## Architecture

The domain is modelled around a clear distinction between a **physical `Seat`** (fixed hardware on a `Screen`) and a **`ShowSeat`** (that seat's booking status *for one particular show*) — the same seat A1 can be booked for the 6 PM show and free for the 9 PM show.

### Class Diagram

[Class Diagram]
<img width="1833" height="913" alt="Untitled Diagram-Class Diagram drawio" src="https://github.com/user-attachments/assets/47ab0fb7-902d-4349-8021-51f9d9da9cbd" />


- **Composition (◆)** — `Cinema`→`Screen`, `Screen`→`Seat`, `Show`→`ShowSeat`: the part has no existence outside the whole.
- **Aggregation (◇)** — `Show`→`Movie`, `Show`→`Screen`, `Booking`→`ShowSeat`, `BookingService`→`Booking`: the part outlives the whole.
- **Association (→)** — `Booking`→`Customer`, `Booking`→`Payment`, `BookingService`→*(PriceCalculator, TicketPrinter, Cinema)*: a reference with no ownership.
- **Inheritance (▷)** — `UpiPayment` / `CardPayment` / `CashPayment` extend the abstract `Payment` contract.

Editable source: [`docs/class_diagram.drawio`](docs/class_diagram.drawio) (open at [app.diagrams.net](https://app.diagrams.net)).

### Sequence Diagram — "book 1 seat & pay by UPI"

[Sequence Diagram]
<img width="1683" height="1298" alt="Untitled Diagram" src="https://github.com/user-attachments/assets/e3e7db3a-0726-49eb-a8fe-6abfbeeacf32" />


## Project Structure

```
.
├── src/
│   ├── 01_Movie.cpp             # movie metadata
│   ├── 02_Seat.cpp              # physical seat + SeatType/pricing
│   ├── 03_Screen.cpp            # auditorium, owns Seats
│   ├── 04_Cinema.cpp            # theatre, owns Screens
│   ├── 05_Show.cpp              # one screening, owns ShowSeats
│   ├── 06_ShowSeat.cpp          # per-show seat status
│   ├── 07_Customer.cpp
│   ├── 08_Booking.cpp
│   ├── 09_Payment.cpp           # abstract payment contract
│   ├── 10_PaymentTypes.cpp      # UpiPayment / CardPayment / CashPayment
│   ├── 11_PriceCalculator.cpp
│   ├── 12_TicketPrinter.cpp
│   ├── 13_BookingService.cpp    # orchestrator
│   └── main.cpp                 # console menu / input reading
├── docs/
└── README.md
```

One class per `.cpp` file, **no header files** — `main.cpp` `#include`s every class file directly, compiling as a single translation unit.

## Getting Started

### Prerequisites
- A C++17-compatible compiler (`g++` ≥ 9, or `clang++` ≥ 10)

### Build & run

```bash
git clone https://github.com/<your-username>/cinebook.git
cd cinebook/src
g++ -std=c++17 -Wall -o booking_system main.cpp
./booking_system
```

No external dependencies — the entire project builds from the C++ standard library.

## Design Principles

The codebase demonstrates all five **SOLID** principles at the class level:

| Principle | Applied as |
|---|---|
| **S**ingle Responsibility | `Booking` stores data and manages its own status; `TicketPrinter` prints; `PriceCalculator` prices — never mixed into one class |
| **O**pen/Closed | Adding a new payment method (e.g. NetBanking) means adding one new `Payment` subclass — zero edits to `BookingService` |
| **L**iskov Substitution | Every `Payment` subclass is usable through a bare `Payment*` with no extra setup — `BookingService` never downcasts |
| **I**nterface Segregation | `Payment` exposes only `pay()` — no subclass is forced to implement an unsupported `refund()` |
| **D**ependency Inversion | `BookingService` depends on the abstract `Payment` interface, never on a concrete payment class |

Full write-up with justification for every principle, plus every class's single responsibility, is in [`docs/SOLID.md`](docs/SOLID.md).

## Edge Cases Handled

- Booking a seat that's already `BOOKED` → rejected, **no partial state change**
-  A failed payment → booking never confirmed, seats released back to `AVAILABLE`
-  Cancelling a booking → seats show `AVAILABLE` again; double-cancellation is rejected cleanly
-  Invalid seat number or menu choice → clear message, **no crash**

## Requirements Reference

- [`docs/REQUIREMENTS.md`](docs/REQUIREMENTS.md) — functional (FR1–FR8) and non-functional requirements
- [`docs/NOUN_VERB_ANALYSIS.md`](docs/NOUN_VERB_ANALYSIS.md) — noun/verb analysis behind the class list
- [`docs/RELATIONSHIPS.md`](docs/RELATIONSHIPS.md) — every class relationship justified with the lifetime test

## Documentation

All design artifacts (requirement analysis, noun-verb table, relationship table, class & sequence diagrams, SOLID mapping, demo transcripts) live under [`docs/`](docs) and are also available as Word documents on request.

## License

This project is released under the [MIT License](LICENSE).
