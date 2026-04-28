#ifndef BUDGETITEM_H
#define BUDGETITEM_H
#include<iostream>
#include <string>
using namespace std;

class BudgetItem {
private:
    int eventId;
    static int nextItemId;
    int itemId;
    string type;
    string category;
    double amount;
    string date;
    string status;
    static string toLower(string str);

public:
    BudgetItem();
    BudgetItem(int eventId, string t, string c, double a, string d, string status);

    void setType(string t);                 //setters
    void setCategory(string c);
    void setAmount(double a);
    void setDate(string d);
    void setStatus(string s);

    int getID() const;                     //getters
    string getType() const;
    string getCategory() const;
    double getAmount() const;
    string getDate() const;
    string getStatus() const;
    int getEventId() const;

    void updateAmount(double a);

    static bool isValidType(string t);
    static bool isValidCategory(string c);
    static bool isValidStatus(string s);
    static bool isValidAmount(double a);

    bool isValidItem() const;
};

#endif