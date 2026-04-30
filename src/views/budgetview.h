#ifndef VIEWS_BUDGETVIEW_H
#define VIEWS_BUDGETVIEW_H

#include <QWidget>

#include "../models/BudgetManager.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;

class BudgetView : public QWidget {
public:
    explicit BudgetView(bool organizerMode, QWidget *parent = nullptr);

private:
    void seedData();
    void rebuildTable();
    void updateSummary();

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

    // Parallel arrays: one entry per EventBudget tracked here for the event filter combo
    // We store event names in the combo itself; we need to map combo index -> eventId.
    // Use a small fixed-size array (max 64 events is plenty for seeded data).
    int eventIds[64];
    int eventCount;
};

#endif
