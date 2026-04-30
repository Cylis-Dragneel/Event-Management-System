#ifndef BUDGETITEM_H
#define BUDGETITEM_H
#include <string>

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

public:
    BudgetItem();
    BudgetItem(int eventId, string t, string c, double a, string d, string status);

    void setType(string t);                 
    void setCategory(string c);
    void setAmount(double a);
    void setDate(string d);
    void setStatus(string s);
    void setEventId(int id);

    int getID() const;                     
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