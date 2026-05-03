#ifndef EVENTBUDGET_H
#define EVENTBUDGET_H

#include <string>
#include "BudgetItem.h"
using namespace std;

class EventBudget {
private:
    int eventId;
    string eventName;
    BudgetItem* items;
    int size;
    int capacity;
    void resizeArray();
    BudgetItem* findItem(int itemId);

public:
    EventBudget();
    EventBudget(int eventId, string name);
    ~EventBudget();

    EventBudget(const EventBudget& other);
    EventBudget& operator=(const EventBudget& other);

    void setEventId(int id);
    void setEventName(string name);

    void addItem(const BudgetItem& item);
    void deleteItem(int itemId);
    void editItem(int itemId, double newAmount);
    void editItem(int itemId, string newCategory);
    void editItem(int itemId, string newType, string newStatus);

    double getTotalIncome()const;
    double getTotalExpense()const;
    double getProfitLoss()const;
    string getProfitStatus()const;
    double getCategoryTotal(string category)const;
    double getHighestExpense()const;

    int filterByType(string type, BudgetItem* results, int maxResults)const;
    int filterByStatus(string status, BudgetItem* results, int maxResults)const;
    int filterByDate(string date, BudgetItem* results, int maxResults)const;
    int searchByCategory(string category, BudgetItem* results, int maxResults)const;

    void sortByAmount();

    void   showHighestExpense()const;
    double getAverageExpense()const;

    int getEventId()const;
    string getEventName()const;
    int getSize()const;
    bool isEmpty()const;

    BudgetItem getItem(int index)const;
};

#endif 
