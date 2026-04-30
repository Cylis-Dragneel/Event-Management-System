#include "BudgetManager.h"
#include "validation.h"
#include<stdexcept>
using namespace std;

static string toLower(string str) {
    for (int i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}

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
    if (event.getEventId() <= 0)
        throw invalid_argument("Cannot add event with invalid ID.");

    if (!Validation::isValidName(event.getEventName()))
        throw invalid_argument("Cannot add event with invalid name.");

    for (int i = 0; i < size; i++) {
        if (events[i].getEventId() == event.getEventId()) {
            throw invalid_argument("Duplicate event ID");
        }
    }
    if (size == capacity) {
        resizeArray();
    }
    events[size++] = event;
}

void BudgetManager::deleteEvent(int eventId) {
    int index = findEventById(eventId);
    if (index == -1) {
        throw invalid_argument("Event not found with ID: " + to_string(eventId));
    }
    for (int i = index; i < size - 1; i++) {
        events[i] = events[i + 1];
    }
    size--;
}

const EventBudget& BudgetManager::getEvent(int eventId)const {
    int index = findEventById(eventId);
    if (index == -1)
        throw invalid_argument("Event not found");
    return events[index];
}

double BudgetManager::getGrandTotalIncome()const {
    double total = 0;
    for (int i = 0; i < size; i++) {
        total += events[i].getTotalIncome();
    }
    return total;
}

double BudgetManager::getGrandTotalExpense()const {
    double total = 0;
    for (int i = 0; i < size; i++) {
        total += events[i].getTotalExpense();
    }
    return total;
}

double BudgetManager::getGrandProfitLoss()const {
    return getGrandTotalIncome() - getGrandTotalExpense();
}

int BudgetManager::getAllEvents(EventBudget* output, int maxSize)const {
    if (output == nullptr) {
        throw invalid_argument("Output array is null");
    }

    int count = size;
    if (maxSize < size) {
        count = maxSize;
    }
    for (int i = 0; i < count; i++) {
        output[i] = events[i];
    }
    return count;
}
int BudgetManager::findEventById(int eventId)const {
    for (int i = 0; i < size; i++) {
        if (events[i].getEventId() == eventId) {
            return i;
        }
    }
    return -1;
}

int BudgetManager::findEventByName(const string& name)const {
    for (int i = 0; i < size; i++) {
        if (toLower(events[i].getEventName()) == toLower(name)) {
            return i;
        }
    }
    return -1;
}

bool BudgetManager::isEmpty()const {
    return size == 0;
}
int  BudgetManager::getSize()const {
    return size;
}

void BudgetManager::clearAll() {
    delete[] events;
    capacity = 3;
    size = 0;
    events = new EventBudget[capacity];
}


