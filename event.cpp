#include "event.h"
#include "database.h"
#include "validation.h"
#include <iostream>

using namespace std;

// ????????????????????????????????????????????????????????????????
//  INSERT EVENT
// ????????????????????????????????????????????????????????????????
bool insertEvent(int organizer_id, int venue_id,
    const string& title, const string& description,
    const string& event_date, int capacity) {

    if (db == nullptr) {
        cerr << "insertEvent error: database is not open." << endl;
        return false;
    }

    if (!isOrganizer(organizer_id)) {
        cerr << "insertEvent denied: user is not an organizer." << endl;
        return false;
    }

    if (!validateEventFields(title, event_date, capacity))
        return false;

    string sql =
        "INSERT INTO Events"
        " (organizer_id, venue_id, title, description, event_date, status, capacity)"
        " VALUES (?, ?, ?, ?, ?, 'Draft', ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "insertEvent prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, organizer_id);
    sqlite3_bind_int(stmt, 2, venue_id);
    sqlite3_bind_text(stmt, 3, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, event_date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, capacity);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "insertEvent error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Event '" << title << "' created successfully." << endl;
    return true;
}

// ????????????????????????????????????????????????????????????????
//  FETCH SINGLE EVENT
// ????????????????????????????????????????????????????????????????
void fetchEvent(int event_id) {

    if (db == nullptr) {
        cerr << "fetchEvent error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT event_id, title, description, event_date, status, capacity"
        " FROM Events WHERE event_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchEvent prepare error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 5);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dText = sqlite3_column_text(stmt, 2);
        const unsigned char* dtText = sqlite3_column_text(stmt, 3);
        const unsigned char* sText = sqlite3_column_text(stmt, 4);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string desc = (dText != nullptr) ?
            reinterpret_cast<const char*>(dText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";

        cout << "=== Event Details ===" << endl;
        cout << "ID       : " << id << endl;
        cout << "Title    : " << title << endl;
        cout << "Desc     : " << desc << endl;
        cout << "Date     : " << date << endl;
        cout << "Status   : " << stat << endl;
        cout << "Capacity : " << cap << endl;

    }
    else {
        cout << "fetchEvent: event " << event_id << " not found." << endl;
    }

    sqlite3_finalize(stmt);
}

// ????????????????????????????????????????????????????????????????
//  FETCH ALL EVENTS
// ????????????????????????????????????????????????????????????????
void fetchAllEvents() {

    if (db == nullptr) {
        cerr << "fetchAllEvents error: database is not open." << endl;
        return;
    }

    const char* sql =
        "SELECT event_id, title, event_date, status, capacity"
        " FROM Events ORDER BY event_id ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchAllEvents prepare error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "=== All Events ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 4);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 2);
        const unsigned char* sText = sqlite3_column_text(stmt, 3);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";

        cout << "ID: " << id
            << " | Title: " << title
            << " | Date: " << date
            << " | Status: " << stat
            << " | Cap: " << cap
            << endl;
    }

    if (!anyRows)
        cout << "No events found." << endl;

    sqlite3_finalize(stmt);
}

// ????????????????????????????????????????????????????????????????
//  UPDATE EVENT STATUS
// ????????????????????????????????????????????????????????????????
bool updateEventStatus(int event_id, const string& newStatus) {

    if (db == nullptr) {
        cerr << "updateEventStatus error: database is not open." << endl;
        return false;
    }

    if (!isValidEventStatus(newStatus)) return false;

    // Get current status
    string current = "";
    {
        string sql = "SELECT status FROM Events WHERE event_id = ?;";
        sqlite3_stmt* stmt = nullptr;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, event_id);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                const unsigned char* sText = sqlite3_column_text(stmt, 0);
                if (sText != nullptr)
                    current = reinterpret_cast<const char*>(sText);
            }
        }
        sqlite3_finalize(stmt);
    }

    if (current.empty()) {
        cerr << "updateEventStatus: event " << event_id
            << " not found." << endl;
        return false;
    }

    // Validate transition
    bool valid = false;
    if (current == "Draft" && newStatus == "Published") valid = true;
    if (current == "Published" && newStatus == "Completed") valid = true;
    if (current == "Published" && newStatus == "Cancelled") valid = true;

    if (!valid) {
        cerr << "Invalid transition: " << current
            << " -> " << newStatus << endl;
        return false;
    }

    string sql = "UPDATE Events SET status = ? WHERE event_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateEventStatus prepare error: "
            << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, newStatus.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, event_id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Event " << event_id
            << " -> '" << newStatus << "'." << endl;
        return true;
    }

    return false;
}

// ????????????????????????????????????????????????????????????????
//  DELETE EVENT
// ????????????????????????????????????????????????????????????????
bool deleteEvent(int event_id) {

    if (db == nullptr) {
        cerr << "deleteEvent error: database is not open." << endl;
        return false;
    }

    string sql =
        "DELETE FROM Events WHERE event_id = ? AND status = 'Draft';";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "deleteEvent prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, event_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Event " << event_id << " deleted." << endl;
        return true;
    }

    cerr << "deleteEvent failed: event " << event_id
        << " not found or not in Draft status." << endl;
    return false;
}

// ????????????????????????????????????????????????????????????????
//  SEARCH AND FILTER HELPERS
// ????????????????????????????????????????????????????????????????

// ??? Search by status ?????????????????????????????????????????????
void searchEventsByStatus(const string& status) {

    if (db == nullptr) {
        cerr << "searchEventsByStatus error: database is not open." << endl;
        return;
    }

    if (!isValidEventStatus(status)) return;

    string sql =
        "SELECT event_id, title, event_date, status, capacity"
        " FROM Events WHERE status = ?"
        " ORDER BY event_date ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "searchEventsByStatus prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);

    cout << "=== Events with status '" << status << "' ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 4);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 2);
        const unsigned char* sText = sqlite3_column_text(stmt, 3);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";

        cout << "ID: " << id
            << " | Title: " << title
            << " | Date: " << date
            << " | Status: " << stat
            << " | Cap: " << cap
            << endl;
    }

    if (!anyRows)
        cout << "No events found with status '"
        << status << "'." << endl;

    sqlite3_finalize(stmt);
}

// ??? Search by date ???????????????????????????????????????????????
void searchEventsByDate(const string& date) {

    if (db == nullptr) {
        cerr << "searchEventsByDate error: database is not open." << endl;
        return;
    }

    if (!isValidDate(date)) return;

    string sql =
        "SELECT event_id, title, event_date, status, capacity"
        " FROM Events WHERE event_date >= ?"
        " ORDER BY event_date ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "searchEventsByDate prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);

    cout << "=== Events on or after '" << date << "' ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 4);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 2);
        const unsigned char* sText = sqlite3_column_text(stmt, 3);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string edate = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";

        cout << "ID: " << id
            << " | Title: " << title
            << " | Date: " << edate
            << " | Status: " << stat
            << " | Cap: " << cap
            << endl;
    }

    if (!anyRows)
        cout << "No events found on or after '"
        << date << "'." << endl;

    sqlite3_finalize(stmt);
}

// ??? Search by title keyword ??????????????????????????????????????
void searchEventsByTitle(const string& keyword) {

    if (db == nullptr) {
        cerr << "searchEventsByTitle error: database is not open." << endl;
        return;
    }

    if (keyword.empty()) {
        cerr << "searchEventsByTitle error: keyword cannot be empty." << endl;
        return;
    }

    string sql =
        "SELECT event_id, title, event_date, status, capacity"
        " FROM Events WHERE LOWER(title) LIKE LOWER(?)"
        " ORDER BY event_date ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "searchEventsByTitle prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    string pattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    cout << "=== Events matching keyword '"
        << keyword << "' ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 4);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 2);
        const unsigned char* sText = sqlite3_column_text(stmt, 3);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";

        cout << "ID: " << id
            << " | Title: " << title
            << " | Date: " << date
            << " | Status: " << stat
            << " | Cap: " << cap
            << endl;
    }

    if (!anyRows)
        cout << "No events found matching '"
        << keyword << "'." << endl;

    sqlite3_finalize(stmt);
}

// ??? Search by organizer ??????????????????????????????????????????
void searchEventsByOrganizer(int organizer_id) {

    if (db == nullptr) {
        cerr << "searchEventsByOrganizer error: database is not open." << endl;
        return;
    }

    if (!isOrganizer(organizer_id)) {
        cerr << "searchEventsByOrganizer error: user "
            << organizer_id << " is not an organizer." << endl;
        return;
    }

    string sql =
        "SELECT e.event_id, e.title, e.event_date,"
        "       e.status,   e.capacity, u.username"
        " FROM Events e"
        " JOIN Users u ON e.organizer_id = u.user_id"
        " WHERE e.organizer_id = ?"
        " ORDER BY e.event_date ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "searchEventsByOrganizer prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, organizer_id);

    cout << "=== Events by organizer ID "
        << organizer_id << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 4);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 2);
        const unsigned char* sText = sqlite3_column_text(stmt, 3);
        const unsigned char* uText = sqlite3_column_text(stmt, 5);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";
        string uname = (uText != nullptr) ?
            reinterpret_cast<const char*>(uText) : "N/A";

        cout << "ID: " << id
            << " | Title: " << title
            << " | Date: " << date
            << " | Status: " << stat
            << " | Cap: " << cap
            << " | Organizer: " << uname
            << endl;
    }

    if (!anyRows)
        cout << "No events found for organizer "
        << organizer_id << "." << endl;

    sqlite3_finalize(stmt);
}

// ??? Filter by capacity range ?????????????????????????????????????
void filterEventsByCapacity(int minCapacity, int maxCapacity) {

    if (db == nullptr) {
        cerr << "filterEventsByCapacity error: database is not open." << endl;
        return;
    }

    if (minCapacity < 0 || maxCapacity < 0) {
        cerr << "filterEventsByCapacity error: "
            << "capacity values cannot be negative." << endl;
        return;
    }

    if (minCapacity > maxCapacity) {
        cerr << "filterEventsByCapacity error: "
            << "minCapacity cannot exceed maxCapacity." << endl;
        return;
    }

    string sql =
        "SELECT event_id, title, event_date, status, capacity"
        " FROM Events WHERE capacity BETWEEN ? AND ?"
        " ORDER BY capacity ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "filterEventsByCapacity prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, minCapacity);
    sqlite3_bind_int(stmt, 2, maxCapacity);

    cout << "=== Events with capacity between "
        << minCapacity << " and " << maxCapacity << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int id = sqlite3_column_int(stmt, 0);
        int cap = sqlite3_column_int(stmt, 4);

        const unsigned char* tText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 2);
        const unsigned char* sText = sqlite3_column_text(stmt, 3);

        string title = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string stat = (sText != nullptr) ?
            reinterpret_cast<const char*>(sText) : "N/A";

        cout << "ID: " << id
            << " | Title: " << title
            << " | Date: " << date
            << " | Status: " << stat
            << " | Cap: " << cap
            << endl;
    }

    if (!anyRows)
        cout << "No events found in that capacity range." << endl;

    sqlite3_finalize(stmt);
}