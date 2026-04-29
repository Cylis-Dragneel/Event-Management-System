#include "EventBudget.h"
#include <iostream>
#include <stdexcept>
using namespace std;

EventBudget::EventBudget() {
    eventId = 0;
    eventName = "Unknown";
    size = 0;
    capacity = 5;
    items = new BudgetItem[capacity];
}

EventBudget::EventBudget(int eventId, string name) {
    this->eventId = eventId;
    eventName = name;
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
    for (int i = 0; i < size; i++) {
        if (items[i].getID() == itemId) {
            items[i].updateAmount(newAmount); // validates inside
            return;
        }
    }
    throw invalid_argument("Item not found: no item with this ID");
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

void EventBudget::showSummary() const {
    cout << " Event: " << eventName << " (ID: " << eventId << ")" << endl;
    cout << "Total Income  : Rs " << getTotalIncome() << endl;
    cout << "Total Expense : Rs " << getTotalExpense() << endl;
    cout << "Profit/Loss   : Rs " << getProfitLoss() << endl;
}

void EventBudget::showProfitStatus() const {
    double p = getProfitLoss();
    if (p > 0)
        cout << "Status: Profit  (Rs " << p << ")" << endl;
    else if (p < 0)
        cout << "Status: Loss    (Rs " << -p << ")" << endl;
    else
        cout << "Status: Break-even" << endl;
}

void EventBudget::showAllItems() const {
    if (size == 0) {
        cout << "No items for event: " << eventName << endl;
        return;
    }
    cout << "--- Budget Items [" << eventName << "]---" << endl;
    for (int i = 0; i < size; i++) {
        cout << "Item ID  : " << items[i].getID() << endl;
        cout << "Type     : " << items[i].getType() << endl;
        cout << "Category : " << items[i].getCategory() << endl;
        cout << "Amount   : Rs " << items[i].getAmount() << endl;
        cout << "Date     : " << items[i].getDate() << endl;
        cout << "Status   : " << items[i].getStatus() << endl;
    }
}

void EventBudget::showCategoryWise() const {
    string categories[100];
    int catCount = 0;
    for (int i = 0; i < size; i++) {
        bool found = false;
        for (int j = 0; j < catCount; j++) {
            if (categories[j] == items[i].getCategory()) {
                found = true;
                break;
            }
        }
        if (!found) {
            categories[catCount++] = items[i].getCategory();
        }
    }
    cout << "--- Category Totals [" << eventName << "] ---" << endl;
    for (int i = 0; i < catCount; i++) {
        double total = 0;
        for (int j = 0; j < size; j++) {
            if (items[j].getCategory() == categories[i]) {
                total += items[j].getAmount();
            }
        }
        cout << categories[i] << " : Rs " << total << endl;
    }
}

void EventBudget::searchByCategory(string category) const {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (items[i].getCategory() == category) {
            cout << "ID: " << items[i].getID()
                << "  Type: " << items[i].getType()
                << "  Amount: Rs " << items[i].getAmount()
                << "  Status: " << items[i].getStatus() << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No items found for category: " << category << endl;
}

void EventBudget::filterByDate(string date) const {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (items[i].getDate() == date) {
            cout << "ID: " << items[i].getID()
                << "  Category: " << items[i].getCategory()
                << "  Amount: Rs " << items[i].getAmount()
                << "  Type: " << items[i].getType() << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No items found for date: " << date << endl;
}

void EventBudget::filterByStatus(string status) const {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (items[i].getStatus() == status) {
            cout << "ID: " << items[i].getID()
                << "  Category: " << items[i].getCategory()
                << "  Amount: Rs " << items[i].getAmount() << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No items with status: " << status << endl;
}

void EventBudget::filterByType(string type) const {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (items[i].getType() == type) {
            cout << "ID: " << items[i].getID()
                << "  Category: " << items[i].getCategory()
                << "  Amount: Rs " << items[i].getAmount()
                << "  Status: " << items[i].getStatus() << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No items of type: " << type << endl;
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

void EventBudget::showHighestExpense() const {
    double maxVal = -1;
    for (int i = 0; i < size; i++) {
        if (items[i].getType() == "expense" &&
            items[i].getAmount() > maxVal) {
            maxVal = items[i].getAmount();
        }
    }
    if (maxVal == -1)
        cout << "No expense items found."<<endl;
    else
        cout << "Highest Expense: Rs " << maxVal << endl;
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
BudgetItem* EventBudget::getItems()const { 
    return items;
}
BudgetItem EventBudget::getItem(int index) const {
    if (index < 0 || index >= size)
        throw out_of_range("Invalid index");
    return items[index];
}
