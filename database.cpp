#include "database.h"
#include <iostream>

using namespace std;

// ??? Global DB handle definition ?????????????????????????????????
sqlite3* db = nullptr;

// ????????????????????????????????????????????????????????????????
//  OPEN DATABASE
// ????????????????????????????????????????????????????????????????
bool openDatabase(const string& path) {

    if (path.empty()) {
        cerr << "openDatabase error: path cannot be empty." << endl;
        return false;
    }

    int rc = sqlite3_open(path.c_str(), &db);

    if (rc != SQLITE_OK) {
        cerr << "openDatabase error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        db = nullptr;
        return false;
    }

    // Enable foreign key enforcement
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    cout << "Database opened successfully." << endl;
    return true;
}

// ????????????????????????????????????????????????????????????????
//  CLOSE DATABASE
// ????????????????????????????????????????????????????????????????
void closeDatabase() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database closed." << endl;
    }
}

// ????????????????????????????????????????????????????????????????
//  CREATE ALL TABLES
// ????????????????????????????????????????????????????????????????
void createTables() {

    if (db == nullptr) {
        cerr << "createTables error: database is not open." << endl;
        return;
    }

    const char* sql =

        // Users
        "CREATE TABLE IF NOT EXISTS Users ("
        "   user_id   INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   username  TEXT NOT NULL UNIQUE,"
        "   password  TEXT NOT NULL,"
        "   email     TEXT NOT NULL UNIQUE,"
        "   role      TEXT NOT NULL CHECK(role IN ('user','organizer'))"
        ");"

        // Venues
        "CREATE TABLE IF NOT EXISTS Venues ("
        "   venue_id  INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   name      TEXT NOT NULL,"
        "   location  TEXT,"
        "   capacity  INTEGER NOT NULL CHECK(capacity > 0)"
        ");"

        // Events
        "CREATE TABLE IF NOT EXISTS Events ("
        "   event_id     INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   organizer_id INTEGER NOT NULL,"
        "   venue_id     INTEGER,"
        "   title        TEXT NOT NULL,"
        "   description  TEXT,"
        "   event_date   TEXT NOT NULL,"
        "   status       TEXT NOT NULL DEFAULT 'Draft'"
        "                CHECK(status IN"
        "                ('Draft','Published','Completed','Cancelled')),"
        "   capacity     INTEGER NOT NULL,"
        "   FOREIGN KEY (organizer_id) REFERENCES Users(user_id),"
        "   FOREIGN KEY (venue_id)     REFERENCES Venues(venue_id)"
        ");"

        // Registrations
        "CREATE TABLE IF NOT EXISTS Registrations ("
        "   reg_id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   event_id       INTEGER NOT NULL,"
        "   user_id        INTEGER NOT NULL,"
        "   status         TEXT NOT NULL DEFAULT 'Confirmed'"
        "                  CHECK(status IN ('Confirmed','Waitlisted','Cancelled')),"
        "   payment_status TEXT NOT NULL DEFAULT 'Pending'"
        "                  CHECK(payment_status IN ('Pending','Paid','Refunded')),"
        "   FOREIGN KEY (event_id) REFERENCES Events(event_id),"
        "   FOREIGN KEY (user_id)  REFERENCES Users(user_id)"
        ");"

        // BudgetItems
        "CREATE TABLE IF NOT EXISTS BudgetItems ("
        "   item_id     INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   event_id    INTEGER NOT NULL,"
        "   category    TEXT NOT NULL,"
        "   type        TEXT NOT NULL CHECK(type IN ('income','expense')),"
        "   amount      REAL NOT NULL CHECK(amount >= 0),"
        "   item_date   TEXT,"
        "   description TEXT,"
        "   FOREIGN KEY (event_id) REFERENCES Events(event_id)"
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "createTables error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "All tables created successfully." << endl;
    }
}

// ????????????????????????????????????????????????????????????????
//  CLEAR ALL DATA (testing only)
// ????????????????????????????????????????????????????????????????
void clearAllData() {

    if (db == nullptr) {
        cerr << "clearAllData error: database is not open." << endl;
        return;
    }

    const char* sql =
        "DELETE FROM BudgetItems;"
        "DELETE FROM Registrations;"
        "DELETE FROM Events;"
        "DELETE FROM Venues;"
        "DELETE FROM Users;"
        "DELETE FROM sqlite_sequence;";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "clearAllData error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "All data cleared. Fresh start." << endl;
    }
}

// ????????????????????????????????????????????????????????????????
//  SEED SAMPLE DATA
// ????????????????????????????????????????????????????????????????
void seedData() {

    if (db == nullptr) {
        cerr << "seedData error: database is not open." << endl;
        return;
    }

    // Only seed if Users table is empty
    int userCount = 0;
    {
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Users;",
            -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW)
                userCount = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    if (userCount > 0) {
        cout << "Seed data already exists. Skipping." << endl;
        return;
    }

    // ?? 1. USERS ?????????????????????????????????????????????????
    // 3 organizers + 6 normal users
    const char* userSQL =
        "INSERT INTO Users (username, password, email, role) VALUES"
        "('alice',   'pass123', 'alice@email.com',   'organizer'),"
        "('bob',     'pass456', 'bob@email.com',     'organizer'),"
        "('carol',   'pass789', 'carol@email.com',   'organizer'),"
        "('david',   'dave01',  'david@email.com',   'user'),"
        "('emma',    'emma01',  'emma@email.com',    'user'),"
        "('frank',   'frank01', 'frank@email.com',   'user'),"
        "('grace',   'grace01', 'grace@email.com',   'user'),"
        "('henry',   'henry01', 'henry@email.com',   'user'),"
        "('isla',    'isla01',  'isla@email.com',    'user');";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, userSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "seedData users error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return;
    }

    // ?? 2. VENUES ????????????????????????????????????????????????
    // 5 venues with different capacities
    const char* venueSQL =
        "INSERT INTO Venues (name, location, capacity) VALUES"
        "('City Hall',        'Downtown',   300),"
        "('Tech Hub',         'Uptown',     100),"
        "('Grand Arena',      'Midtown',    500),"
        "('Community Center', 'Eastside',    80),"
        "('Rooftop Lounge',   'Westside',    40);";

    if (sqlite3_exec(db, venueSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "seedData venues error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return;
    }

    // ?? 3. EVENTS ????????????????????????????????????????????????
    // Mix of all statuses and different organizers
    const char* eventSQL =
        "INSERT INTO Events"
        " (organizer_id, venue_id, title, description, event_date, status, capacity)"
        " VALUES"

        // Alice's events (organizer 1)
        "(1, 3, 'Annual Gala',"
        " 'Yearly charity gala with dinner and auction.',"
        " '2025-09-15', 'Published', 400),"

        "(1, 2, 'Tech Workshop',"
        " 'Hands-on workshop covering latest web technologies.',"
        " '2025-10-20', 'Draft', 80),"

        "(1, 1, 'New Year Celebration',"
        " 'Welcome 2026 with live music and fireworks.',"
        " '2025-12-31', 'Published', 250),"

        // Bob's events (organizer 2)
        "(2, 4, 'Art Exhibition',"
        " 'Local artists showcase their latest works.',"
        " '2025-08-10', 'Completed', 70),"

        "(2, 5, 'Startup Pitch Night',"
        " 'Entrepreneurs pitch ideas to investors.',"
        " '2025-11-05', 'Published', 35),"

        // Carol's events (organizer 3)
        "(3, 1, 'Career Fair',"
        " 'Connect students with top employers.',"
        " '2025-09-30', 'Published', 200),"

        "(3, 2, 'Photography Bootcamp',"
        " 'Beginner to advanced photography techniques.',"
        " '2025-07-01', 'Cancelled', 60);";

    if (sqlite3_exec(db, eventSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "seedData events error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return;
    }

    // ?? 4. REGISTRATIONS ?????????????????????????????????????????
    // Spread across different events with mixed statuses
    const char* regSQL =
        "INSERT INTO Registrations (event_id, user_id, status, payment_status)"
        " VALUES"

        // Annual Gala (event 1) — 4 confirmed, 1 waitlisted
        "(1, 4, 'Confirmed',  'Paid'),"
        "(1, 5, 'Confirmed',  'Paid'),"
        "(1, 6, 'Confirmed',  'Pending'),"
        "(1, 7, 'Confirmed',  'Pending'),"
        "(1, 8, 'Waitlisted', 'Pending'),"

        // Tech Workshop (event 2) — 2 confirmed
        "(2, 4, 'Confirmed', 'Paid'),"
        "(2, 9, 'Confirmed', 'Pending'),"

        // Art Exhibition (event 4) — completed event, all paid
        "(4, 5, 'Confirmed', 'Paid'),"
        "(4, 6, 'Confirmed', 'Paid'),"
        "(4, 7, 'Confirmed', 'Paid'),"

        // Startup Pitch Night (event 5) — small venue, 1 waitlisted
        "(5, 4, 'Confirmed',  'Paid'),"
        "(5, 5, 'Confirmed',  'Paid'),"
        "(5, 8, 'Waitlisted', 'Pending'),"

        // Career Fair (event 6) — mix of statuses
        "(6, 4, 'Confirmed',  'Paid'),"
        "(6, 5, 'Confirmed',  'Pending'),"
        "(6, 6, 'Cancelled',  'Refunded'),"
        "(6, 9, 'Confirmed',  'Pending');";

    if (sqlite3_exec(db, regSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "seedData registrations error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return;
    }

    // ?? 5. BUDGET ITEMS ??????????????????????????????????????????
    // Realistic income and expense entries per event
    const char* budgetSQL =
        "INSERT INTO BudgetItems"
        " (event_id, category, type, amount, item_date, description)"
        " VALUES"

        // Annual Gala (event 1)
        "(1, 'Venue Rental',  'expense', 3000.00, '2025-08-01', 'City Hall booking fee'),"
        "(1, 'Catering',      'expense', 2000.00, '2025-08-15', 'Dinner for 400 guests'),"
        "(1, 'Marketing',     'expense',  500.00, '2025-08-20', 'Social media and flyers'),"
        "(1, 'Ticket Sales',  'income',  8000.00, '2025-09-01', 'Online ticket revenue'),"
        "(1, 'Sponsorship',   'income',  3000.00, '2025-09-05', 'Brand sponsorship deal'),"

        // Tech Workshop (event 2)
        "(2, 'Venue Rental',  'expense',  800.00, '2025-09-01', 'Tech Hub booking fee'),"
        "(2, 'Equipment',     'expense',  300.00, '2025-09-10', 'Projector and cables'),"
        "(2, 'Ticket Sales',  'income',  2000.00, '2025-09-15', 'Workshop tickets'),"

        // Art Exhibition (event 4) — completed event
        "(4, 'Venue Rental',  'expense',  600.00, '2025-07-01', 'Community Center fee'),"
        "(4, 'Decorations',   'expense',  200.00, '2025-07-10', 'Frames and lighting'),"
        "(4, 'Ticket Sales',  'income',  1500.00, '2025-07-15', 'Entry tickets'),"
        "(4, 'Artwork Sales', 'income',  3000.00, '2025-08-10', 'Commission from sales'),"

        // Startup Pitch Night (event 5)
        "(5, 'Venue Rental',  'expense',  400.00, '2025-10-01', 'Rooftop Lounge fee'),"
        "(5, 'Catering',      'expense',  250.00, '2025-10-05', 'Snacks and drinks'),"
        "(5, 'Ticket Sales',  'income',   700.00, '2025-10-10', 'Attendee tickets'),"
        "(5, 'Sponsorship',   'income',  1000.00, '2025-10-12', 'Investor sponsorship'),"

        // Career Fair (event 6)
        "(6, 'Venue Rental',  'expense', 1500.00, '2025-09-01', 'City Hall main hall'),"
        "(6, 'Marketing',     'expense',  400.00, '2025-09-05', 'Campus posters and ads'),"
        "(6, 'Staff',         'expense',  600.00, '2025-09-10', 'Volunteer coordination'),"
        "(6, 'Booth Fees',    'income',  5000.00, '2025-09-15', 'Company booth payments'),"
        "(6, 'Sponsorship',   'income',  2000.00, '2025-09-18', 'University sponsorship');";

    if (sqlite3_exec(db, budgetSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "seedData budget error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return;
    }

    cout << "Seed data inserted successfully." << endl;
    cout << "  - 9 users  (3 organizers + 6 attendees)" << endl;
    cout << "  - 5 venues (capacities: 40 to 500)" << endl;
    cout << "  - 7 events (Draft/Published/Completed/Cancelled)" << endl;
    cout << "  - 17 registrations (Confirmed/Waitlisted/Cancelled)" << endl;
    cout << "  - 21 budget items  (income + expenses)" << endl;
}

// ????????????????????????????????????????????????????????????????
//  ROLE HELPERS
// ????????????????????????????????????????????????????????????????
string getUserRole(int user_id) {

    if (db == nullptr) {
        cerr << "getUserRole error: database is not open." << endl;
        return "";
    }

    string role = "";
    string sql = "SELECT role FROM Users WHERE user_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text != nullptr)
                role = reinterpret_cast<const char*>(text);
        }
    }

    sqlite3_finalize(stmt);
    return role;
}

bool isOrganizer(int user_id) { return getUserRole(user_id) == "organizer"; }
bool isUser(int user_id) { return getUserRole(user_id) == "user"; }