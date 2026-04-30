#ifndef VIEWS_BUDGETVIEW_H
#define VIEWS_BUDGETVIEW_H

#include <QWidget>

class QComboBox;
class QDateEdit;
class QLabel;
class QPushButton;
class QTableWidget;

class BudgetView : public QWidget {
public:
    explicit BudgetView(bool organizerMode, QWidget *parent = nullptr);

private:
    bool isOrganizerMode;
    QComboBox *eventFilter;
    QComboBox *typeFilter;
    QDateEdit *fromDate;
    QDateEdit *toDate;
    QTableWidget *budgetTable;
    QLabel *incomeValue;
    QLabel *expenseValue;
    QLabel *profitValue;
    QPushButton *addIncomeButton;
    QPushButton *addExpenseButton;
    QPushButton *deleteItemButton;
    QPushButton *refreshButton;
};

#endif
