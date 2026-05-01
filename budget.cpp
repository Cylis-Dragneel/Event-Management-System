#include "budget.h"
#include "database.h"
#include "validation.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ════════════════════════════════════════════════════════════════
//  INSERT BUDGET ITEM
// ════════════════════════════════════════════════════════════════
bool insertBudgetItem(int event_id, const string& category,
    const string& type, double amount,
    const string& item_date,
    const string& description) {

    if (db == nullptr) {
        cerr << "insertBudgetItem error: database is not open." << endl;
        return false;
    }

    if (!validateBudgetFields(category, type, amount, item_date))
        return false;

    // Check event exists
    {
        string checkSql =
            "SELECT COUNT(*) FROM Events WHERE event_id = ?;";
        sqlite3_stmt* checkStmt = nullptr;
        int count = 0;

        if (sqlite3_prepare_v2(db, checkSql.c_str(), -1,
            &checkStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(checkStmt, 1, event_id);
            if (sqlite3_step(checkStmt) == SQLITE_ROW)
                count = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);

        if (count == 0) {
            cerr << "insertBudgetItem error: event "
                << event_id << " does not exist." << endl;
            return false;
        }
    }

    string sql =
        "INSERT INTO BudgetItems"
        " (event_id, category, type, amount, item_date, description)"
        " VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "insertBudgetItem prepare error: "
            << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, event_id);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, amount);
    sqlite3_bind_text(stmt, 5, item_date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, description.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "insertBudgetItem error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Budget item '" << category << "' ("
        << type << ": $"
        << fixed << setprecision(2) << amount
        << ") added to event " << event_id << "." << endl;
    return true;
}

// ════════════════════════════════════════════════════════════════
//  FETCH ALL BUDGET ITEMS
// ════════════════════════════════════════════════════════════════
void fetchBudgetItems(int event_id) {

    if (db == nullptr) {
        cerr << "fetchBudgetItems error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT item_id, category, type, amount, item_date, description"
        " FROM BudgetItems WHERE event_id = ?"
        " ORDER BY type, item_id;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchBudgetItems prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    cout << "=== Budget Items for Event "
        << event_id << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int    id = sqlite3_column_int(stmt, 0);
        double amt = sqlite3_column_double(stmt, 3);

        const unsigned char* cText = sqlite3_column_text(stmt, 1);
        const unsigned char* tText = sqlite3_column_text(stmt, 2);
        const unsigned char* dtText = sqlite3_column_text(stmt, 4);
        const unsigned char* dText = sqlite3_column_text(stmt, 5);

        string cat = (cText != nullptr) ?
            reinterpret_cast<const char*>(cText) : "N/A";
        string type = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string desc = (dText != nullptr) ?
            reinterpret_cast<const char*>(dText) : "N/A";

        cout << "ItemID: " << id
            << " | Category: " << cat
            << " | Type: " << type
            << " | Amount: $" << fixed << setprecision(2) << amt
            << " | Date: " << date
            << " | Desc: " << desc
            << endl;
    }

    if (!anyRows)
        cout << "No budget items found for event "
        << event_id << "." << endl;

    sqlite3_finalize(stmt);
}

// ════════════════════════════════════════════════════════════════
//  UPDATE BUDGET ITEM
// ════════════════════════════════════════════════════════════════
bool updateBudgetItem(int item_id, const string& category,
    const string& type, double amount,
    const string& item_date,
    const string& description) {

    if (db == nullptr) {
        cerr << "updateBudgetItem error: database is not open." << endl;
        return false;
    }

    if (!validateBudgetFields(category, type, amount, item_date))
        return false;

    string sql =
        "UPDATE BudgetItems"
        " SET category = ?, type = ?, amount = ?,"
        "     item_date = ?, description = ?"
        " WHERE item_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateBudgetItem prepare error: "
            << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, amount);
    sqlite3_bind_text(stmt, 4, item_date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, item_id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Budget item " << item_id
            << " updated successfully." << endl;
        return true;
    }

    cerr << "updateBudgetItem: item "
        << item_id << " not found." << endl;
    return false;
}

// ════════════════════════════════════════════════════════════════
//  DELETE BUDGET ITEM
// ════════════════════════════════════════════════════════════════
bool deleteBudgetItem(int item_id) {

    if (db == nullptr) {
        cerr << "deleteBudgetItem error: database is not open." << endl;
        return false;
    }

    string sql = "DELETE FROM BudgetItems WHERE item_id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "deleteBudgetItem prepare error: "
            << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, item_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) > 0) {
        cout << "Budget item " << item_id << " deleted." << endl;
        return true;
    }

    cerr << "deleteBudgetItem: item "
        << item_id << " not found." << endl;
    return false;
}

// ════════════════════════════════════════════════════════════════
//  FINANCIAL SUMMARY
// ════════════════════════════════════════════════════════════════
void fetchFinancialSummary(int event_id) {

    if (db == nullptr) {
        cerr << "fetchFinancialSummary error: database is not open." << endl;
        return;
    }

    double totalIncome = 0.0;
    {
        string sql =
            "SELECT COALESCE(SUM(amount), 0)"
            " FROM BudgetItems"
            " WHERE event_id = ? AND type = 'income';";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, event_id);
            if (sqlite3_step(stmt) == SQLITE_ROW)
                totalIncome = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    double totalExpenses = 0.0;
    {
        string sql =
            "SELECT COALESCE(SUM(amount), 0)"
            " FROM BudgetItems"
            " WHERE event_id = ? AND type = 'expense';";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, event_id);
            if (sqlite3_step(stmt) == SQLITE_ROW)
                totalExpenses = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    double profitLoss = totalIncome - totalExpenses;
    string result = (profitLoss >= 0) ? "PROFIT" : "LOSS";

    cout << "=== Financial Summary for Event "
        << event_id << " ===" << endl;
    cout << fixed << setprecision(2);
    cout << "Total Income   : $" << totalIncome << endl;
    cout << "Total Expenses : $" << totalExpenses << endl;
    cout << "---------------------------------" << endl;
    cout << result << "         : $"
        << abs(profitLoss) << endl;
}

// ════════════════════════════════════════════════════════════════
//  CATEGORY HELPERS
// ════════════════════════════════════════════════════════════════

void fetchBudgetByCategory(int event_id, const string& category) {

    if (db == nullptr) {
        cerr << "fetchBudgetByCategory error: database is not open." << endl;
        return;
    }

    if (category.empty()) {
        cerr << "fetchBudgetByCategory error: category cannot be empty." << endl;
        return;
    }

    string sql =
        "SELECT item_id, category, type, amount, item_date, description"
        " FROM BudgetItems"
        " WHERE event_id = ? AND LOWER(category) = LOWER(?)"
        " ORDER BY item_id ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchBudgetByCategory prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);

    cout << "=== Budget items for event " << event_id
        << " | Category: " << category << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int    id = sqlite3_column_int(stmt, 0);
        double amt = sqlite3_column_double(stmt, 3);

        const unsigned char* cText = sqlite3_column_text(stmt, 1);
        const unsigned char* tText = sqlite3_column_text(stmt, 2);
        const unsigned char* dtText = sqlite3_column_text(stmt, 4);
        const unsigned char* dText = sqlite3_column_text(stmt, 5);

        string cat = (cText != nullptr) ?
            reinterpret_cast<const char*>(cText) : "N/A";
        string type = (tText != nullptr) ?
            reinterpret_cast<const char*>(tText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string desc = (dText != nullptr) ?
            reinterpret_cast<const char*>(dText) : "N/A";

        cout << "ItemID: " << id
            << " | Category: " << cat
            << " | Type: " << type
            << " | Amount: $" << fixed << setprecision(2) << amt
            << " | Date: " << date
            << " | Desc: " << desc
            << endl;
    }

    if (!anyRows)
        cout << "No items found in category '"
        << category << "'." << endl;

    sqlite3_finalize(stmt);
}

void fetchIncomeItems(int event_id) {

    if (db == nullptr) {
        cerr << "fetchIncomeItems error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT item_id, category, amount, item_date, description"
        " FROM BudgetItems"
        " WHERE event_id = ? AND type = 'income'"
        " ORDER BY item_id ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchIncomeItems prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    cout << "=== Income items for event "
        << event_id << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int    id = sqlite3_column_int(stmt, 0);
        double amt = sqlite3_column_double(stmt, 2);

        const unsigned char* cText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 3);
        const unsigned char* dText = sqlite3_column_text(stmt, 4);

        string cat = (cText != nullptr) ?
            reinterpret_cast<const char*>(cText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string desc = (dText != nullptr) ?
            reinterpret_cast<const char*>(dText) : "N/A";

        cout << "ItemID: " << id
            << " | Category: " << cat
            << " | Amount: $" << fixed << setprecision(2) << amt
            << " | Date: " << date
            << " | Desc: " << desc
            << endl;
    }

    if (!anyRows)
        cout << "No income items found for event "
        << event_id << "." << endl;

    sqlite3_finalize(stmt);
}

void fetchExpenseItems(int event_id) {

    if (db == nullptr) {
        cerr << "fetchExpenseItems error: database is not open." << endl;
        return;
    }

    string sql =
        "SELECT item_id, category, amount, item_date, description"
        " FROM BudgetItems"
        " WHERE event_id = ? AND type = 'expense'"
        " ORDER BY item_id ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "fetchExpenseItems prepare error: "
            << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    cout << "=== Expense items for event "
        << event_id << " ===" << endl;

    bool anyRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        anyRows = true;

        int    id = sqlite3_column_int(stmt, 0);
        double amt = sqlite3_column_double(stmt, 2);

        const unsigned char* cText = sqlite3_column_text(stmt, 1);
        const unsigned char* dtText = sqlite3_column_text(stmt, 3);
        const unsigned char* dText = sqlite3_column_text(stmt, 4);

        string cat = (cText != nullptr) ?
            reinterpret_cast<const char*>(cText) : "N/A";
        string date = (dtText != nullptr) ?
            reinterpret_cast<const char*>(dtText) : "N/A";
        string desc = (dText != nullptr) ?
            reinterpret_cast<const char*>(dText) : "N/A";

        cout << "ItemID: " << id
            << " | Category: " << cat
            << " | Amount: $" << fixed << setprecision(2) << amt
            << " | Date: " << date
            << " | Desc: " << desc
            << endl;
    }

    if (!anyRows)
        cout << "No expense items found for event "
        << event_id << "." << endl;

    sqlite3_finalize(stmt);
}