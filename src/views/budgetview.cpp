#include "budgetview.h"

#include "dialogs/budgetitemdialog.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QFormLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

BudgetView::BudgetView(bool organizerMode, QWidget *parent)
    : QWidget(parent),
      isOrganizerMode(organizerMode),
      eventFilter(new QComboBox(this)),
      typeFilter(new QComboBox(this)),
      fromDate(new QDateEdit(this)),
      toDate(new QDateEdit(this)),
      budgetTable(new QTableWidget(this)),
      incomeValue(new QLabel("$0.00", this)),
      expenseValue(new QLabel("$0.00", this)),
      profitValue(new QLabel("$0.00", this)),
      addIncomeButton(new QPushButton("Add Income", this)),
      addExpenseButton(new QPushButton("Add Expense", this)),
      deleteItemButton(new QPushButton("Delete Item", this)),
      refreshButton(new QPushButton("Refresh", this)) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *filtersLayout = new QFormLayout();
    eventFilter->addItems({"All Events"});
    typeFilter->addItems({"All Types", "Income", "Expense"});
    filtersLayout->addRow("Event", eventFilter);
    filtersLayout->addRow("Item Type", typeFilter);

    fromDate->setCalendarPopup(true);
    toDate->setCalendarPopup(true);
    fromDate->setDate(QDate::currentDate().addMonths(-1));
    toDate->setDate(QDate::currentDate().addMonths(1));
    filtersLayout->addRow("From", fromDate);
    filtersLayout->addRow("To", toDate);
    mainLayout->addLayout(filtersLayout);

    auto *summaryLayout = new QGridLayout();
    summaryLayout->addWidget(new QLabel("Income", this), 0, 0);
    summaryLayout->addWidget(incomeValue, 0, 1);
    summaryLayout->addWidget(new QLabel("Expenses", this), 1, 0);
    summaryLayout->addWidget(expenseValue, 1, 1);
    summaryLayout->addWidget(new QLabel("Profit/Loss", this), 2, 0);
    summaryLayout->addWidget(profitValue, 2, 1);
    mainLayout->addLayout(summaryLayout);

    budgetTable->setColumnCount(7);
    budgetTable->setHorizontalHeaderLabels(
        {"Date", "Event", "Type", "Category", "Description", "Amount", "Payment Status"});
    budgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    budgetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    budgetTable->setSelectionMode(QAbstractItemView::SingleSelection);
    budgetTable->setSortingEnabled(true);
    mainLayout->addWidget(budgetTable);

    auto *actionsLayout = new QHBoxLayout();
    actionsLayout->addWidget(refreshButton);
    actionsLayout->addStretch();
    actionsLayout->addWidget(addIncomeButton);
    actionsLayout->addWidget(addExpenseButton);
    actionsLayout->addWidget(deleteItemButton);
    mainLayout->addLayout(actionsLayout);

    addIncomeButton->setEnabled(isOrganizerMode);
    addExpenseButton->setEnabled(isOrganizerMode);
    deleteItemButton->setEnabled(isOrganizerMode);

    QObject::connect(addIncomeButton, &QPushButton::clicked, this, [this]() {
        BudgetItemDialog dialog("Income", this);
        dialog.exec();
    });

    QObject::connect(addExpenseButton, &QPushButton::clicked, this, [this]() {
        BudgetItemDialog dialog("Expense", this);
        dialog.exec();
    });

    QObject::connect(deleteItemButton, &QPushButton::clicked, this, [this]() {
        if (budgetTable->currentRow() < 0) {
            QMessageBox::information(this, "Delete Budget Item", "Select a budget item row first.");
            return;
        }
        QMessageBox::information(this, "Delete Budget Item", "Delete flow will be connected to controller logic.");
    });
}

