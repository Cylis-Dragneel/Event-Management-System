# Event Management System

2nd Semester OOP Project — FAST

A desktop event management application built with C++17, Qt 6, and SQLite. Supports organizer and attendee roles with separate views for each.

---

## Features

### Authentication
- **Login** — Existing users authenticate with username and password
- **Sign Up** — New accounts can be created as Attendee or Organizer
- **Role-based access** — Two separate UI flows after login:
  - **Organizer view**: Full access to Events, Venues, Registrations, and Budgets
  - **Attendee view**: Browse events, register, view own registrations

### Event Management (Organizer)
- **Create / Edit / Delete** events with full details: name, description, date/time, duration, type, capacity, venue, status
- **Event types**: Conference, Workshop, Concert, Wedding, Corporate, Social
- **Status workflow**: Draft → Published → Completed / Cancelled
- **Registration fee** per event (stored alongside event details)
- **Search / Filter** by name, date range, type, and status
- **Sortable** table view with cost column

### Venue Management (Organizer)
- **Create / Edit / Delete** venues: name, address, capacity, hourly rate, contact info, description
- **Amenities**: WiFi, Parking, Catering, AV Equipment, Accessibility, Stage, Tables/Chairs

### Registration Management
- **Organizer**: Register attendees on behalf of others (select event from live list), confirm registrations, export to CSV
- **Attendee**: Self-register for published events from the Browse Events tab
- **Registration details** (name, email) stored directly with each record
- **Statuses**: Pending, Confirmed, Cancelled, Waitlisted
- **CSV export** of attendee data

### Payment System
- **Make payments** on registrations with support for partial payments
- **Payment status** auto-updates: Unpaid → Partial → Paid
- **Balance tracking** shown per registration
- **Refund** support in the model layer

### Budget Tracking (Organizer)
- **Add income / expense** items per event from dynamically loaded event list
- **Categories**: Venue, Catering, Marketing, Staff, Equipment, Other
- **Financial summary**: Total income, total expenses, profit/loss
- **Filter** by event and type (income/expense)
- **Sortable** table view

### General
- **Dark / Light theme** toggle
- **SQLite** database for persistent local storage
- **Material Design** styling via Qt Style Sheets
- **MVC architecture**

---

## Dependencies

| Dependency | Required Version |
|---|---|
| C++ Compiler | C++17 support |
| CMake | ≥ 3.16 |
| Qt | 6 (Core, Widgets, Sql, Gui) |
| SQLite | Provided via Qt SQL module |

---

## Build & Run

### Linux

#### 1. Install dependencies

**Ubuntu / Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-base-dev-tools
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake qt6-base
```

**Fedora:**
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel
```

#### 2. Clone & configure
```bash
cd Event-Management-System
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

#### 3. Build
```bash
cmake --build build -j$(nproc)
```

#### 4. Run
```bash
./build/EventManagementSystem
```

---

### Windows

#### 1. Install dependencies

1. **Visual Studio Build Tools** (or Visual Studio 2022 Community)
   - Download: https://visualstudio.microsoft.com/downloads/
   - Select "Desktop development with C++"

2. **CMake**
   - Download: https://cmake.org/download/
   - Add to PATH during installation

3. **Qt 6**
   - Download: https://www.qt.io/download (Qt Online Installer)
   - In the installer, select: Qt 6.x → MSVC 2022 64-bit (or MinGW 64-bit)
   - Also select: Qt → Qt SQL Drivers → SQLite (usually included by default)

4. **Set up Qt in environment** (for Developer Command Prompt):
   ```cmd
   set PATH=C:\Qt\6.x.x\msvc2022_64\bin;%PATH%
   ```
   Or use CMake with the Qt path explicitly:
   ```cmd
   cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2022_64"
   ```

#### 2. Clone & configure
Open **Developer Command Prompt for VS 2022** or any terminal with CMake and Qt in PATH:
```cmd
cd Event-Management-System
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

#### 3. Build
```cmd
cmake --build build --config Release
```

#### 4. Run
```cmd
build\Release\EventManagementSystem.exe
```

---

## Project Structure

```
Event-Management-System/
├── CMakeLists.txt
├── README.md
├── resources/
│   └── resources.qrc           # Material + Dark theme QSS stylesheets
└── src/
    ├── main.cpp
    ├── app/
    │   ├── application.cpp     # App entry, creates MainWindow + Database
    │   └── stylesheetmanager.cpp
    ├── models/
    │   ├── database.cpp/.h     # SQLite setup, CRUD for all entities
    │   ├── event.cpp/.h
    │   ├── venue.cpp/.h
    │   ├── user.cpp/.h
    │   ├── Registration.cpp/.h
    │   ├── Attendee.cpp/.h
    │   ├── BudgetItem.cpp/.h
    │   ├── EventBudget.cpp/.h
    │   ├── BudgetManager.cpp/.h
    │   ├── CSVExporter.cpp/.h
    │   ├── validation.cpp/.h
    │   └── ...
    └── views/
        ├── mainwindow.cpp/.h
        ├── eventview.cpp/.h
        ├── venueview.cpp/.h
        ├── registrationview.cpp/.h
        ├── budgetview.cpp/.h
        └── dialogs/
            ├── eventdialog.cpp/.h
            ├── venuedialog.cpp/.h
            ├── registrationdialog.cpp/.h
            ├── paymentdialog.cpp/.h
            ├── budgetitemdialog.cpp/.h
            ├── logindialog.cpp/.h
            └── signupdialog.cpp/.h
```
