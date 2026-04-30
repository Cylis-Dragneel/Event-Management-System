#ifndef BUDGETMANAGER_H
#define BUDGETMANAGER_H
#include "EventBudget.h"
#include <string>
using namespace std;

class BudgetManager {
private:
    EventBudget* events;
    int size;
    int capacity;
    void resizeArray();
public:
    BudgetManager();
    ~BudgetManager();

    BudgetManager(const BudgetManager& other);
    BudgetManager& operator=(const BudgetManager& other);

    void addEvent(const EventBudget& event);
    void deleteEvent(int eventId);

    void showEvent(int eventId)const;
    void showAllSummaries()const;
    void showGrandTotal()const;

    int  findEventById(int eventId)const;
    int  findEventByName(string name) const;

    bool isEmpty()const;
    void clearAll();
    int  getSize()const;

    const EventBudget* getEvents()const; 
};

#endif