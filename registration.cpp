#include "registration.h"
#include "database.h"
#include "validation.h"
#include <iostream>
#include <fstream>

using namespace std;

// ????????????????????????????????????????????????????????????????
//  PRIVATE HELPERS
// ????????????????????????????????????????????????????????????????

static int countConfirmed(int event_id) {

    string sql =
        "SELECT COUNT(*) FROM Registrations"
        " WHERE event_id = ? AND status = 'Confirmed';";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, event_id);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

static int getEventCapacity(int event_id) {

    string sql = "SELECT capacity FROM Events WHERE event_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    int capacity = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, event_id);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            capacity = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return capacity;
}

static bool alreadyRegistered(int event_id, int user_id) {

    string sql =
        "SELECT COUNT(*) FROM Registrations"
        " WHERE event_id = ? AND user_id = ?"
        " AND status != 'Cancelled';";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, event_id);
        sqlite3_bind_int(stmt, 2, user_id);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count > 0;
}

// ????????????????????????????????????????????????????????????????
//  REGISTER ATTENDEE
// ????????????????????????????????????????????????????????????????
bool registerAttendee(int event_id, int user_id) {

    if (db == nullptr) {
        cerr << "registerAttendee error: database is not open." << endl;
        return false;
    }

    int capacity = getEventCapacity(event_id);
    if (capacity == 0) {
        cerr << "registerAttendee: event not found." << endl;
        return false;
    }

    if (alreadyRegistered(event_id, user_id)) {
        cerr << "registerAttendee: user " << user_id
            << " is already registered for event "
            << event_id << endl;
        return false;
    }

    int    confirmed = countConfirmed(event_id);
    string status = (confirmed < capacity) ? "Confirmed" : "Waitlisted";

    string sql =
        "INSERT INTO Registrations (event_id, user_id, status, payment_status)"
        " VALUES (?, ?, ?, 'Pending');";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "registerAttendee prepare error: "
            << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, event_id);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_text(stmt, 3, status.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "registerAttendee error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "User " << user_id
        << " registered for event " << event_id
        << " | Status: " << status << endl;
    return true;
}

// ????????????????????????????????????????????????????????????????
//  CANCEL REGISTRATION + AUTO PROMOTE
// ????????????????????????????????????????????????????????????????
bool cancelRegistration(int event_id, int user_id) {

    if (db == nullptr) {
        cerr << "cancelRegistration error: database is not open." << endl;
        return false;
    }

    string currentStatus = "";
    {
        string sql =
            "SELECT status FROM Registrations"
            " WHERE event_id = ? AND user_id = ?"
            " AND status != 'Cancelled';";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, event_id);
            sqlite3_bind_int(stmt, 2, user_id);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                const unsigned char* sText = sqlite3_column_text(stmt, 0);
                if (sText != nullptr)
                    currentStatus = reinterpret_cast<const char*>(sText);
            }
        }
        sqlite3_finalize(stmt);
    }

    if (currentStatus.empty()) {
        cerr << "cancelRegistration: active registration not found." << endl;
        return false;
    }

    {
        string sql =
            "UPDATE Registrations SET status = 'Cancelled'"
            " WHERE event_id = ? AND user_id = ?"
            " AND status != 'Cancelled';";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "cancelRegistration prepare error: "
                << sqlite3_errmsg(db) << endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, event_id);
        sqlite3_bind_int(stmt, 2, user_id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    cout << "User " << user_id
        << " registration cancelled for event "
        << event_id << "." << endl;

    // Auto promote first waitlisted user
    if (currentStatus == "Confirmed") {

        int waitlist_reg_id = -1;
        {
            string sql =
                "SELECT reg_id FROM Registrations"
                " WHERE event_id = ? AND status = 'Waitlisted'"
                " ORDER BY reg_id ASC LIMIT 1;";

            sqlite3_stmt* stmt = nullptr;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, event_id);
                if (sqlite3_step(stmt) == SQLITE_ROW)
                    waitlist_reg_id = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }

        if (waitlist_reg_id != -1) {
            string sql =
                "UPDATE Registrations SET status = 'Confirmed'"
                " WHERE reg_id = ?;";

            sqlite3_stmt* stmt = nullptr;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, waitlist_reg_id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                cout << "Waitlisted registration " << waitlist_reg_id
                    << " promoted to Confirmed." << endl;
            }
        }
    }

    return true;
}

// ????????????????????????????????????????????????????????????????
//  UPDATE PAYMENT STATUS
// ????????????????????????????????????????????????????????????????
bool updatePaymentStatus(int reg_id, const string& payment_status) {

    if (db == nullptr) {
        cerr << "updatePaymentStatus error: database is not open." << endl;
        return false;
    }

    if (!isValidPaymentStatus(payment_status)) return false;

    string sql =
        "UPDATE Registrations SET payment_status = ?"
        " WHERE reg_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updatePaymentStatus prepare error: "
            << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, payment_status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, reg_id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Registration " << reg_id
            << " payment -> " << payment_status << endl;
        return true;
    }

    cerr << "updatePaymentStatus: registration "
        << reg_id << " not found." << endl;
    return false;
}

// ????????????????????????????????????????????????????????????????
//  FETCH REGISTRATIONS
// ????????????????????????????????????????????????????????????????
void fetchRegistration(int event_id) {

    if (db == nullptr) {
        cerr << "fetchRegistration error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT r.reg_id, u.username, r.status, r.payment_status"
        " FROM Registrations r"
        " JOIN Users u ON r.user_id = u.user_id"
        " WHERE r.event_id = ?"
        " ORDER BY r.reg_id ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchRegistration prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    cout << "=== Registrations for Event "
        << event_id << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int reg_id = sqlite3_column_int(stmt, 0);

        const unsigned char* uText = sqlite3_column_text(stmt, 1);
        const unsigned char* sText = sqlite3_column_text(stmt, 2);
        const unsigned char* pText = sqlite3_column_text(stmt, 3);

        string uname = (uText != nullptr) ?
            reinterpret_cast<const char*>(uText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";
        string pay = (pText != nullptr) ?
            reinterpret_cast<const char*>(pText) : "N/A";

        cout << "RegID: " << reg_id
            << " | User: " << uname
            << " | Status: " << stat
            << " | Pay: " << pay
            << endl;
    }

    if (!anyRows)
        cout << "No registrations found for event "
        << event_id << "." << endl;

    sqlite3_finalize(stmt);
}

// ????????????????????????????????????????????????????????????????
//  EXPORT CSV
// ????????????????????????????????????????????????????????????????
void exportAttendeesCSV(int event_id, const string& filename) {

    if (db == nullptr) {
        cerr << "exportAttendeesCSV error: database is not open." << endl;
        return;
    }

    if (filename.empty()) {
        cerr << "exportAttendeesCSV error: filename cannot be empty." << endl;
        return;
    }

    string sql =
        "SELECT u.username, u.email, r.status, r.payment_status"
        " FROM Registrations r"
        " JOIN Users u ON r.user_id = u.user_id"
        " WHERE r.event_id = ?"
        " ORDER BY r.reg_id ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "exportAttendeesCSV prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "exportAttendeesCSV: could not open file "
            << filename << endl;
        sqlite3_finalize(stmt);
        return;
    }

    file << "Username,Email,Status,PaymentStatus\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        const unsigned char* uText = sqlite3_column_text(stmt, 0);
        const unsigned char* eText = sqlite3_column_text(stmt, 1);
        const unsigned char* sText = sqlite3_column_text(stmt, 2);
        const unsigned char* pText = sqlite3_column_text(stmt, 3);

        string uname = (uText != nullptr) ?
            reinterpret_cast<const char*>(uText) : "";
        string email = (eText != nullptr) ?
            reinterpret_cast<const char*>(eText) : "";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "";
        string pay = (pText != nullptr) ?
            reinterpret_cast<const char*>(pText) : "";

        file << uname << ","
            << email << ","
            << stat << ","
            << pay << "\n";
    }

    sqlite3_finalize(stmt);
    file.close();

    cout << "Attendees exported to "
        << filename << " successfully." << endl;
}

// ????????????????????????????????????????????????????????????????
//  COUNT HELPERS
// ????????????????????????????????????????????????????????????????

int getConfirmedCount(int event_id) {

    if (db == nullptr) {
        cerr << "getConfirmedCount error: database is not open." << endl;
        return -1;
    }

    string sql =
        "SELECT COUNT(*) FROM Registrations"
        " WHERE event_id = ? AND status = 'Confirmed';";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, event_id);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    cout << "Event " << event_id
        << " confirmed count: " << count << endl;
    return count;
}

int getWaitlistCount(int event_id) {

    if (db == nullptr) {
        cerr << "getWaitlistCount error: database is not open." << endl;
        return -1;
    }

    string sql =
        "SELECT COUNT(*) FROM Registrations"
        " WHERE event_id = ? AND status = 'Waitlisted';";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, event_id);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    cout << "Event " << event_id
        << " waitlist count: " << count << endl;
    return count;
}

bool isEventFull(int event_id) {

    if (db == nullptr) {
        cerr << "isEventFull error: database is not open." << endl;
        return false;
    }

    int confirmed = countConfirmed(event_id);
    int capacity = getEventCapacity(event_id);

    bool full = (confirmed >= capacity);

    cout << "Event " << event_id
        << (full ? " is FULL." : " has spots available.")
        << endl;

    return full;
}