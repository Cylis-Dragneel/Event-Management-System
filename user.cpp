#include "user.h"
#include "database.h"
#include <iostream>

using namespace std;

// ─── Insert new user ─────────────────────────────────────────────
bool insertUser(const string& username, const string& password,
    const string& email, const string& role) {

    if (username.empty() || password.empty() || email.empty()) {
        cerr << "insertUser error: fields cannot be empty." << endl;
        return false;
    }

    if (role != "user" && role != "organizer") {
        cerr << "insertUser error: role must be 'user' or 'organizer'." << endl;
        return false;
    }

    if (db == nullptr) {
        cerr << "insertUser error: database is not open." << endl;
        return false;
    }

    string sql =
        "INSERT INTO Users (username, password, email, role)"
        " VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "insertUser prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "insertUser error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "User '" << username << "' inserted successfully." << endl;
    return true;
}

// ─── Login fetch ─────────────────────────────────────────────────
bool loginUser(const string& username, const string& password,
    int& out_user_id, string& out_role) {

    out_user_id = -1;
    out_role = "";

    if (username.empty() || password.empty()) {
        cerr << "loginUser error: username and password cannot be empty." << endl;
        return false;
    }

    if (db == nullptr) {
        cerr << "loginUser error: database is not open." << endl;
        return false;
    }

    string sql =
        "SELECT user_id, role FROM Users"
        " WHERE username = ? AND password = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "loginUser prepare error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        out_user_id = sqlite3_column_int(stmt, 0);

        const unsigned char* roleText = sqlite3_column_text(stmt, 1);
        if (roleText != nullptr)
            out_role = reinterpret_cast<const char*>(roleText);

        found = true;
    }

    sqlite3_finalize(stmt);

    if (found)
        cout << "Login successful. Role: " << out_role << endl;
    else
        cout << "Login failed. Invalid credentials." << endl;

    return found;
}

// ─── Fetch user by ID ────────────────────────────────────────────
void fetchUser(int user_id) {

    if (db == nullptr) {
        cerr << "fetchUser error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT user_id, username, email, role"
        " FROM Users WHERE user_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchUser prepare error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        // safely read each column with null checks
        int         id = sqlite3_column_int(stmt, 0);

        const unsigned char* uText = sqlite3_column_text(stmt, 1);
        const unsigned char* eText = sqlite3_column_text(stmt, 2);
        const unsigned char* rText = sqlite3_column_text(stmt, 3);

        string uname = (uText != nullptr) ? reinterpret_cast<const char*>(uText) : "N/A";
        string email = (eText != nullptr) ? reinterpret_cast<const char*>(eText) : "N/A";
        string role = (rText != nullptr) ? reinterpret_cast<const char*>(rText) : "N/A";

        cout << "=== User Details ===" << endl;
        cout << "ID       : " << id << endl;
        cout << "Username : " << uname << endl;
        cout << "Email    : " << email << endl;
        cout << "Role     : " << role << endl;

    }
    else {
        cout << "fetchUser: user " << user_id << " not found." << endl;
    }

    sqlite3_finalize(stmt);
}

// ─── Check if user exists by ID ──────────────────────────────────
bool userExists(int user_id) {

    if (db == nullptr) {
        cerr << "userExists error: database is not open." << endl;
        return false;
    }

    string sql = "SELECT COUNT(*) FROM Users WHERE user_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}