#include "venue.h"
#include "database.h"
#include <iostream>

using namespace std;

// ??? Insert new venue ????????????????????????????????????????????
bool insertVenue(const string& name, const string& location, int capacity) {

    if (db == nullptr) {
        cerr << "insertVenue error: database is not open." << endl;
        return false;
    }

    if (name.empty()) {
        cerr << "insertVenue error: name cannot be empty." << endl;
        return false;
    }

    if (capacity <= 0) {
        cerr << "insertVenue error: capacity must be greater than 0." << endl;
        return false;
    }

    string sql =
        "INSERT INTO Venues (name, location, capacity)"
        " VALUES (?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "insertVenue prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, location.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, capacity);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "insertVenue error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Venue '" << name << "' inserted successfully." << endl;
    return true;
}

// ??? Fetch single venue ??????????????????????????????????????????
void fetchVenue(int venue_id) {

    if (db == nullptr) {
        cerr << "fetchVenue error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT venue_id, name, location, capacity"
        " FROM Venues WHERE venue_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchVenue prepare error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, venue_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 3);

        const unsigned char* nText = sqlite3_column_text(stmt, 1);
        const unsigned char* lText = sqlite3_column_text(stmt, 2);

        string name = (nText != nullptr) ? reinterpret_cast<const char*>(nText) : "N/A";
        string loc = (lText != nullptr) ? reinterpret_cast<const char*>(lText) : "N/A";

        cout << "=== Venue Details ===" << endl;
        cout << "ID       : " << id << endl;
        cout << "Name     : " << name << endl;
        cout << "Location : " << loc << endl;
        cout << "Capacity : " << cap << endl;

    }
    else {
        cout << "fetchVenue: venue " << venue_id << " not found." << endl;
    }

    sqlite3_finalize(stmt);
}

// ??? Fetch all venues ????????????????????????????????????????????
void fetchAllVenues() {

    if (db == nullptr) {
        cerr << "fetchAllVenues error: database is not open." << endl;
        return;
    }

    const char* sql =
        "SELECT venue_id, name, location, capacity FROM Venues;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchAllVenues prepare error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "=== All Venues ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 3);

        const unsigned char* nText = sqlite3_column_text(stmt, 1);
        const unsigned char* lText = sqlite3_column_text(stmt, 2);

        string name = (nText != nullptr) ? reinterpret_cast<const char*>(nText) : "N/A";
        string loc = (lText != nullptr) ? reinterpret_cast<const char*>(lText) : "N/A";

        cout << "ID: " << id
            << " | Name: " << name
            << " | Location: " << loc
            << " | Capacity: " << cap
            << endl;
    }

    if (!anyRows)
        cout << "No venues found." << endl;

    sqlite3_finalize(stmt);
}

// ??? Update venue ????????????????????????????????????????????????
bool updateVenue(int venue_id, const string& name,
    const string& location, int capacity) {

    if (db == nullptr) {
        cerr << "updateVenue error: database is not open." << endl;
        return false;
    }

    if (name.empty()) {
        cerr << "updateVenue error: name cannot be empty." << endl;
        return false;
    }

    if (capacity <= 0) {
        cerr << "updateVenue error: capacity must be greater than 0." << endl;
        return false;
    }

    string sql =
        "UPDATE Venues SET name = ?, location = ?, capacity = ?"
        " WHERE venue_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateVenue prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, location.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, capacity);
    sqlite3_bind_int(stmt, 4, venue_id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Venue " << venue_id << " updated successfully." << endl;
        return true;
    }

    cerr << "updateVenue: venue " << venue_id << " not found." << endl;
    return false;
}

// ??? Delete venue ????????????????????????????????????????????????
bool deleteVenue(int venue_id) {

    if (db == nullptr) {
        cerr << "deleteVenue error: database is not open." << endl;
        return false;
    }

    // Block delete if venue is linked to any event
    {
        string sql = "SELECT COUNT(*) FROM Events WHERE venue_id = ?;";
        sqlite3_stmt* stmt = nullptr;
        int count = 0;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, venue_id);
            if (sqlite3_step(stmt) == SQLITE_ROW)
                count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);

        if (count > 0) {
            cerr << "deleteVenue denied: venue is linked to "
                << count << " event(s)." << endl;
            return false;
        }
    }

    string sql = "DELETE FROM Venues WHERE venue_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "deleteVenue prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, venue_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Venue " << venue_id << " deleted." << endl;
        return true;
    }

    cerr << "deleteVenue: venue " << venue_id << " not found." << endl;
    return false;
}