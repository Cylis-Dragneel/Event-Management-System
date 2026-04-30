#include "EventBudget.h"
#include "validation.h"
#include <stdexcept>

string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
    return str;
}

BudgetItem* EventBudget::findItem(int itemId) {
    for (int i = 0; i < size; i++) {
        if (items[i].getID() == itemId)
            return &items[i];
    }
    return nullptr;
}

EventBudget::EventBudget() {
    eventId = 0;
    eventName = "Unknown";
    size = 0;
    capacity = 5;
    items = new BudgetItem[capacity];
}

EventBudget::EventBudget(int eventId, string name) {
    setEventId(eventId);
    setEventName(name);
    size = 0;
    capacity = 5;
    items = new BudgetItem[capacity];
}

EventBudget::~EventBudget() {
    delete[] items;
}

EventBudget::EventBudget(const EventBudget& other) {
    eventId = other.eventId;
    eventName = other.eventName;
    size = other.size;
    capacity = other.capacity;

    items = new BudgetItem[capacity];
    for (int i = 0; i < size; i++) {
        items[i] = other.items[i];
    }
}

EventBudget& EventBudget::operator=(const EventBudget& other) {
    if (this == &other) {
        return *this;
    }
    delete[] items;
    eventId = other.eventId;
    eventName = other.eventName;
    size = other.size;
    capacity = other.capacity;

    items = new BudgetItem[capacity];
    for (int i = 0; i < size; i++) {
        items[i] = other.items[i];
    }
    return *this;
}

void EventBudget::setEventId(int id) {
    if (id <= 0)
        throw invalid_argument("Event ID must be greater than 0.");
    eventId = id;
}

void EventBudget::setEventName(string name) {
    if (!Validation::isValidName(name))
        throw invalid_argument("Invalid event name.");
    eventName = name;
}

void EventBudget::resizeArray() {
    capacity *= 2;
    BudgetItem* newArr = new BudgetItem[capacity];
    for (int i = 0; i < size; i++) {
        newArr[i] = items[i];
    }
    delete[] items;
    items = newArr;
}

void EventBudget::addItem(const BudgetItem& item) {
    if (!item.isValidItem()) {
        throw invalid_argument("Cannot add: item data is invalid");
    }
    for (int i = 0; i < size; i++) {
        if (items[i].getID() == item.getID()) {
            throw invalid_argument("Duplicate item ID");
        }
    }
    if (size == capacity) {
        resizeArray();
    }
    items[size++] = item;
}

void EventBudget::deleteItem(int itemId) {
    for (int i = 0; i < size; i++) {
        if (items[i].getID() == itemId) {
            for (int j = i; j < size - 1; j++) {
                items[j] = items[j + 1];
            }
            size--;
            return;
        }
    }
    throw invalid_argument("Item not found: no item with this ID");
}

void EventBudget::editItem(int itemId, double newAmount) {
    BudgetItem* item = findItem(itemId);
    if (!item) throw invalid_argument("Item not found.");
    item->setAmount(newAmount);
}

void EventBudget::editItem(int itemId, string newCategory) {
    BudgetItem* item = findItem(itemId);
    if (!item) throw invalid_argument("Item not found.");
    item->setCategory(newCategory);
}

void EventBudget::editItem(int itemId, string newType, string newStatus) {
    BudgetItem* item = findItem(itemId);
    if (!item) throw invalid_argument("Item not found.");
    item->setType(newType);
    item->setStatus(newStatus);
}

double EventBudget::getTotalIncome() const {
    double total = 0;
    for (int i = 0; i < size; i++) {
        if (items[i].getType() == "income") {
            total += items[i].getAmount();
        }
    }
    return total;
}

double EventBudget::getTotalExpense() const {
    double total = 0;
    for (int i = 0; i < size; i++) {
        if (items[i].getType() == "expense") {
            total += items[i].getAmount();
        }
    }
    return total;
}

double EventBudget::getProfitLoss() const {
    return getTotalIncome() - getTotalExpense();
}

string EventBudget::getProfitStatus() const {
    double p = getProfitLoss();
    if (p > 0)  return "Profit";
    if (p < 0)  return "Loss";
    return "Break-even";
}

double EventBudget::getCategoryTotal(string category) const {
    string lower = toLower(category);
    if (lower != "venue" && lower != "catering" && lower != "marketing" && lower != "staff" &&
        lower != "equipment" && lower != "other") {
        throw invalid_argument("Invalid category!");
    }

    double sum = 0;
    for (int i = 0; i < size; i++) {
        if (items[i].getCategory() == lower)
            sum += items[i].getAmount();
    }
    return sum;
}

double EventBudget::getHighestExpense() const {
    bool found = false;
    double maxVal = 0;
    for (int i = 0; i < size; i++) {
        if (items[i].getType() == "expense") {
            if (!found || items[i].getAmount() > maxVal) {
                maxVal = items[i].getAmount();
                found = true;
            }
        }
    }
    return maxVal; 
}

int EventBudget::searchByCategory(string category, BudgetItem* results, int maxResults) const {
    string lower = toLower(category);
    if (lower != "venue" && lower != "catering" && lower != "marketing" && lower != "staff"
        && lower != "equipment" && lower != "other") {
        throw invalid_argument("Invalid category!");
    }

    int count = 0;
    for (int i = 0; i < size && count < maxResults; i++) {
        if (items[i].getCategory() == lower)
            results[count++] = items[i];
    }
    return count;
}

int EventBudget::filterByDate(string date, BudgetItem* results, int maxResults) const {
    if (!Validation::isValidDate(date)) {
        throw invalid_argument("Invalid date format! Expected DD-MM-YYYY.");
    }

    int count = 0;
    for (int i = 0; i < size && count < maxResults; i++) {
        if (items[i].getDate() == date)
            results[count++] = items[i];
    }
    return count;
}

int EventBudget::filterByStatus(string status, BudgetItem* results, int maxResults) const {
    string lower = toLower(status);
    if (lower != "pending" && lower != "paid") {
        throw invalid_argument("Status must be 'pending' or 'paid'.");
    }

    int count = 0;
    for (int i = 0; i < size && count < maxResults; i++) {
        if (items[i].getStatus() == lower)
            results[count++] = items[i];
    }
    return count;
}

int EventBudget::filterByType(string type, BudgetItem* results, int maxResults) const {
    string lower = toLower(type);
    if (lower != "income" && lower != "expense")
        throw invalid_argument("Type must be 'income' or 'expense'.");

    int count = 0;
    for (int i = 0; i < size && count < maxResults; i++) {
        if (items[i].getType() == lower)
            results[count++] = items[i];
    }
    return count;
}

void EventBudget::sortByAmount() {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (items[j].getAmount() > items[i].getAmount()) {
                BudgetItem temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }
        }
    }
}

double EventBudget::getAverageExpense() const {
    double total = 0;
    int    count = 0;
    for (int i = 0; i < size; i++) {
        if (items[i].getType() == "expense") {
            total += items[i].getAmount();
            count++;
        }
    }
    if (count == 0) return 0;
    return total / count;
}

int EventBudget::getEventId()const {
    return eventId;
}
string EventBudget::getEventName()const {
    return eventName;
}
int EventBudget::getSize()const {
    return size;
}
bool EventBudget::isEmpty()const {
    return size == 0;
}

BudgetItem EventBudget::getItem(int index) const {
    if (index < 0 || index >= size)
        throw out_of_range("Invalid index");
    return items[index];
}
