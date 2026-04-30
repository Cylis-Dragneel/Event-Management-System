#ifndef BUDGETMANAGER_H
#define BUDGETMANAGER_H
#include "EventBudget.h"
#include <string>
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

    const EventBudget& getEvent(int eventId) const;
    double getGrandTotalIncome()  const;
    double getGrandTotalExpense() const;
    double getGrandProfitLoss()   const;

    int  findEventById(int eventId)const;
    int  findEventByName(const string& name) const;
    int getAllEvents(EventBudget* output, int maxSize) const;

    bool isEmpty()const;
    void clearAll();
    int  getSize()const;
};

#endif