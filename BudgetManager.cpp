#include "BudgetManager.h"
#include <iostream>
using namespace std;

BudgetManager::BudgetManager() {
    size = 0;
    capacity = 3;
    events = new EventBudget[capacity];
}

BudgetManager::~BudgetManager() {
    delete[] events;
}

BudgetManager::BudgetManager(const BudgetManager& other) {
    size = other.size;
    capacity = other.capacity;
    events = new EventBudget[capacity];
    for (int i = 0; i < size; i++) {
        events[i] = other.events[i];  
    }
}

BudgetManager& BudgetManager::operator=(const BudgetManager& other) {
    if (this == &other) {
        return *this;
    }
    delete[] events;
    size = other.size;
    capacity = other.capacity;
    events = new EventBudget[capacity];
    for (int i = 0; i < size; i++) {
        events[i] = other.events[i];
    }
    return *this;
}

void BudgetManager::resizeArray() {
    capacity *= 2;
    EventBudget* newArr = new EventBudget[capacity];
    for (int i = 0; i < size; i++) {
        newArr[i] = events[i];
    }
    delete[] events;
    events = newArr;
}

void BudgetManager::addEvent(const EventBudget& event) {
    for (int i = 0; i < size; i++) {
        if (events[i].getEventId() == event.getEventId()) {
            throw invalid_argument("Duplicate event ID");
        }
    }
    if (size == capacity) {
        resizeArray();
    }
    events[size++] = event;
    cout << "Event '" << event.getEventName() << "' added."<<endl;
}

void BudgetManager::deleteEvent(int eventId) {
    int index = findEventById(eventId);
    if (index == -1) {
        cout << "Event not found with ID: " << eventId << endl;
        return;
    }
    for (int i = index; i < size - 1; i++) {
        events[i] = events[i + 1];
    }
    size--;
    cout << "Event deleted."<<endl;
}

void BudgetManager::showEvent(int eventId) const {
    int index = findEventById(eventId);

    if (index == -1) {
        cout << "Event not found with ID: " << eventId << endl;
        return;
    }
    events[index].showSummary();
    events[index].showAllItems();
}

void BudgetManager::showAllSummaries() const {
    if (size == 0) {
        cout << "No events available."<<endl;
        return;
    }
    for (int i = 0; i < size; i++) {
        events[i].showSummary();
    }
}

void BudgetManager::showGrandTotal() const {
    double totalIncome = 0;
    double totalExpense = 0;
    for (int i = 0; i < size; i++) {
        totalIncome += events[i].getTotalIncome();
        totalExpense += events[i].getTotalExpense();
    }

    cout << "--- GRAND TOTAL (All Events) ---";
    cout << "Total Income  : Rs " << totalIncome << endl;
    cout << "Total Expense : Rs " << totalExpense << endl;
    cout << "Net P/L       : Rs " << (totalIncome - totalExpense) << endl;
}

int BudgetManager::findEventById(int eventId) const {
    for (int i = 0; i < size; i++) {
        if (events[i].getEventId() == eventId) {
            return i;
        }
    }
    return -1;
}

int BudgetManager::findEventByName(string name) const {
    for (int i = 0; i < size; i++) {
        if (events[i].getEventName() == name) {
            return i;
        }
    }
    return -1;
}

bool BudgetManager::isEmpty()  const {
    return size == 0; 
}
int  BudgetManager::getSize()  const { 
    return size;
}

void BudgetManager::clearAll() {
    delete[] events;
    capacity = 3;
    size = 0;
    events = new EventBudget[capacity];
    cout << "All events cleared."<<endl;
}

EventBudget* BudgetManager::getEvents() const { 
    return events; 
}
