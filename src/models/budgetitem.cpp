#include "BudgetItem.h"
#include "validation.h"
#include <stdexcept>
using namespace std;

int BudgetItem::nextItemId = 1;

Static string BudgetItem::toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
    return str;
}
BudgetItem::BudgetItem() {
    eventId = 0;
    itemId = nextItemId++;
    type = "income";
    category = "other";
    amount = 1.0;
    date = "01-01-2000";
    status = "pending";
}

BudgetItem::BudgetItem(int eventId, string t, string c, double a, string d, string status) {
    itemId = nextItemId++;
    setStatus(status);
    setType(t);
    setCategory(c);
    setAmount(a);
    setDate(d);
    setEventId(eventId);
}

int BudgetItem::getID() const { 
    return itemId;
}
string BudgetItem::getType() const {
    return type;
}
string BudgetItem::getCategory() const {
    return category; 
}
double BudgetItem::getAmount() const {
    return amount;
}
string BudgetItem::getDate() const {
    return date; 
}
string BudgetItem::getStatus() const {
    return status;
}
int BudgetItem::getEventId() const { 
    return eventId; 
}

void BudgetItem::setType(string t) {
    if (!BudgetItem::isValidType(t)) {
        throw invalid_argument("Invalid Type");
    }
    type = toLower(t);
}

void BudgetItem::setCategory(string c) {
    if (!BudgetItem::isValidCategory(c)) {
        throw invalid_argument("Invalid Category");
    }
    category = toLower(c);
}

void BudgetItem::setAmount(double a) {
    if (!BudgetItem::isValidAmount(a)) {
        throw invalid_argument("Invalid Amount");
    }
    amount = a;
}

void BudgetItem::setDate(string d) {
    if (!Validation::isValidDate(d)) {
        throw invalid_argument("Invalid Date");
    }
    date = d;
}

void BudgetItem::setStatus(string s) {
    if (!BudgetItem::isValidStatus(s)) {
        throw invalid_argument("Invalid Status");
    }
    status = toLower(s);
}
void BudgetItem::updateAmount(double a) {
    setAmount(a);
}

void BudgetItem::setEventId(int id) {
    if (id <= 0) {
        throw invalid_argument("Event ID must be greater than 0.");
    }
    eventId = id;
}
bool BudgetItem::isValidType(string t) {
    t = toLower(t);
    if (t == "income" || t == "expense") {
        return true;
    }
    return false;
}

bool BudgetItem::isValidCategory(string c) {
    c = toLower(c);
    if (c == "venue" || c == "catering" || c == "marketing" || c == "staff" || c == "equipment" || c == "other") {
        return true;
    }
    return false;
}

bool BudgetItem::isValidStatus(string s) {
    s = toLower(s);
    if (s == "pending" || s == "paid") {
        return true;
    }
    return false;
}

bool BudgetItem::isValidAmount(double a) {
    return (a > 0);
}

bool BudgetItem::isValidItem() const {
    if (status == "paid" && amount <= 0) {
        return false;
    }

    return BudgetItem::isValidType(type) && BudgetItem::isValidAmount(amount) && BudgetItem::isValidCategory(category) && BudgetItem::isValidStatus(status) &&Validation::isValidDate(date);
}