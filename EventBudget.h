#ifndef EVENTBUDGET_H
#define EVENTBUDGET_H
#include<iostream>
#include <string>
#include "BudgetItem.h"
using namespace std;

class EventBudget {
private:
    int   eventId;      
    string eventName;    
    BudgetItem* items;   
    int size;           
    int capacity;       
    void resizeArray();  

public:
    EventBudget();
    EventBudget(int eventId, string name);
    ~EventBudget();

    EventBudget(const EventBudget& other);
    EventBudget& operator=(const EventBudget& other);

    void addItem(const BudgetItem& item);    
    void deleteItem(int itemId);            
    void editItem(int itemId, double newAmount); 

    double getTotalIncome()  const;
    double getTotalExpense() const;
    double getProfitLoss()   const;

    void showSummary()      const;   
    void showProfitStatus() const;   
    void showAllItems()     const;   
    void showCategoryWise() const;   

    void searchByCategory(string category) const;
    void filterByDate(string date)         const;
    void filterByStatus(string status)     const; 
    void filterByType(string type)         const; 

    void sortByAmount();  

    void   showHighestExpense() const;
    double getAverageExpense()  const;

    int    getEventId()   const;
    string getEventName() const;
    int    getSize()      const;
    bool   isEmpty()      const;

    BudgetItem  getItem(int index)  const; 
    BudgetItem* getItems()          const; 
};

#endif 
