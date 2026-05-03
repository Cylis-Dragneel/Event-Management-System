#ifndef VIEWS_BUDGETVIEW_H
#define VIEWS_BUDGETVIEW_H

#include <QWidget>
#include <QMap>

#include "../models/BudgetManager.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class Database;

class BudgetView : public QWidget {
public:
    explicit BudgetView(Database *database, bool organizerMode, QWidget *parent = nullptr);

private:
    void loadFromDatabase();
    void rebuildTable();
    void updateSummary();

    Database *database;
    bool isOrganizerMode;
    QComboBox    *eventFilter;
    QComboBox    *typeFilter;
    QTableWidget *budgetTable;
    QLabel       *incomeValue;
    QLabel       *expenseValue;
    QLabel       *profitValue;
    QPushButton  *addIncomeButton;
    QPushButton  *addExpenseButton;
    QPushButton  *deleteItemButton;
    QPushButton  *refreshButton;

    BudgetManager manager;

    QMap<int, QString> eventNames;
};

#endif
