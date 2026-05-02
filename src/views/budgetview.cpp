#include "budgetview.h"

#include "dialogs/budgetitemdialog.h"
#include "../models/database.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

BudgetView::BudgetView(Database *db, bool organizerMode, QWidget *parent)
    : QWidget(parent),
      database(db),
      isOrganizerMode(organizerMode),
      eventFilter(new QComboBox(this)),
      typeFilter(new QComboBox(this)),
      budgetTable(new QTableWidget(this)),
      incomeValue(new QLabel("$0.00", this)),
      expenseValue(new QLabel("$0.00", this)),
      profitValue(new QLabel("$0.00", this)),
      addIncomeButton(new QPushButton("Add Income", this)),
      addExpenseButton(new QPushButton("Add Expense", this)),
      deleteItemButton(new QPushButton("Delete Item", this)),
      refreshButton(new QPushButton("Refresh", this)),
      eventCount(0) {
    auto *mainLayout = new QVBoxLayout(this);

    // --- Filters ---
    auto *filtersLayout = new QHBoxLayout();
    eventFilter->addItem("All Events");
    typeFilter->addItems({"All Types", "income", "expense"});
    filtersLayout->addWidget(new QLabel("Event:", this));
    filtersLayout->addWidget(eventFilter);
    filtersLayout->addWidget(new QLabel("Type:", this));
    filtersLayout->addWidget(typeFilter);
    filtersLayout->addStretch();
    mainLayout->addLayout(filtersLayout);

    // --- Summary ---
    auto *summaryLayout = new QGridLayout();
    summaryLayout->addWidget(new QLabel("Total Income:", this), 0, 0);
    summaryLayout->addWidget(incomeValue, 0, 1);
    summaryLayout->addWidget(new QLabel("Total Expenses:", this), 1, 0);
    summaryLayout->addWidget(expenseValue, 1, 1);
    summaryLayout->addWidget(new QLabel("Profit / Loss:", this), 2, 0);
    summaryLayout->addWidget(profitValue, 2, 1);
    mainLayout->addLayout(summaryLayout);

    // --- Table ---
    budgetTable->setColumnCount(6);
    budgetTable->setHorizontalHeaderLabels({"Event", "Type", "Category", "Amount", "Date", "Status"});
    budgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    budgetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    budgetTable->setSelectionMode(QAbstractItemView::SingleSelection);
    budgetTable->setSortingEnabled(true);
    mainLayout->addWidget(budgetTable);

    // --- Actions ---
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

    loadFromDatabase();
    rebuildTable();
    updateSummary();

    // Populate event filter combo from manager
    eventFilter->clear();
    eventFilter->addItem("All Events");
    EventBudget allEvents[64];
    int total = manager.getAllEvents(allEvents, 64);
    for (int i = 0; i < total; i++) {
        eventFilter->addItem(QString::fromStdString(allEvents[i].getEventName()));
    }

    QObject::connect(refreshButton, &QPushButton::clicked, this, [this]() {
        rebuildTable();
        updateSummary();
    });

    QObject::connect(eventFilter, &QComboBox::currentIndexChanged, this, [this](int) {
        rebuildTable();
        updateSummary();
    });

    QObject::connect(typeFilter, &QComboBox::currentIndexChanged, this, [this](int) {
        rebuildTable();
        updateSummary();
    });

    QObject::connect(addIncomeButton, &QPushButton::clicked, this, [this]() {
        BudgetItemDialog dialog("income", this);

        QStringList names;
        for (int i = 0; i < eventCount; i++) {
            EventBudget allEvents[64];
            int total = manager.getAllEvents(allEvents, 64);
            for (int j = 0; j < total; j++) {
                if (allEvents[j].getEventId() == eventIds[i]) {
                    names << QString::fromStdString(allEvents[j].getEventName());
                    break;
                }
            }
        }
        dialog.setEvents(names);

        if (dialog.exec() == QDialog::Accepted) {
            int idx = dialog.getEventIndex();
            if (idx < 0 || idx >= eventCount) return;
            int targetEventId = eventIds[idx];

            BudgetItem item(targetEventId,
                            dialog.getType().toStdString(),
                            dialog.getCategory().toStdString(),
                            dialog.getAmount(),
                            dialog.getDate().toStdString(),
                            dialog.getStatus().toStdString());

            EventBudget allEvts[64];
            int total = manager.getAllEvents(allEvts, 64);
            for (int i = 0; i < total; i++) {
                if (allEvts[i].getEventId() == targetEventId) {
                    EventBudget updated = allEvts[i];
                    updated.addItem(item);
                    manager.deleteEvent(targetEventId);
                    manager.addEvent(updated);
                    break;
                }
            }
            rebuildTable();
            updateSummary();
        }
    });

    QObject::connect(addExpenseButton, &QPushButton::clicked, this, [this]() {
        BudgetItemDialog dialog("expense", this);

        QStringList names;
        EventBudget allEvts[64];
        int total = manager.getAllEvents(allEvts, 64);
        for (int j = 0; j < total; j++) {
            names << QString::fromStdString(allEvts[j].getEventName());
        }
        dialog.setEvents(names);

        if (dialog.exec() == QDialog::Accepted) {
            int idx = dialog.getEventIndex();
            if (idx < 0 || idx >= eventCount) return;
            int targetEventId = eventIds[idx];

            BudgetItem item(targetEventId,
                            dialog.getType().toStdString(),
                            dialog.getCategory().toStdString(),
                            dialog.getAmount(),
                            dialog.getDate().toStdString(),
                            dialog.getStatus().toStdString());

            EventBudget allEvtsInner[64];
            int totalInner = manager.getAllEvents(allEvtsInner, 64);
            for (int i = 0; i < totalInner; i++) {
                if (allEvtsInner[i].getEventId() == targetEventId) {
                    EventBudget updated = allEvtsInner[i];
                    updated.addItem(item);
                    manager.deleteEvent(targetEventId);
                    manager.addEvent(updated);
                    break;
                }
            }
            rebuildTable();
            updateSummary();
        }
    });

    QObject::connect(deleteItemButton, &QPushButton::clicked, this, [this]() {
        int row = budgetTable->currentRow();
        if (row < 0) {
            QMessageBox::information(this, "Delete Item", "Select a budget item row first.");
            return;
        }

        // itemId and eventId are stored as UserRole data on column 0
        QTableWidgetItem *cell = budgetTable->item(row, 0);
        if (!cell) return;
        int itemId  = cell->data(Qt::UserRole).toInt();
        int evtId   = cell->data(Qt::UserRole + 1).toInt();

        EventBudget allEvts[64];
        int total = manager.getAllEvents(allEvts, 64);
        for (int i = 0; i < total; i++) {
            if (allEvts[i].getEventId() == evtId) {
                EventBudget updated = allEvts[i];
                updated.deleteItem(itemId);
                manager.deleteEvent(evtId);
                manager.addEvent(updated);
                break;
            }
        }
        rebuildTable();
        updateSummary();
    });
}

void BudgetView::loadFromDatabase() {
    if (!database) {
        return;
    }

    int count = 0;
    BudgetItem *items = database->getAllBudgetItems(count);

    QMap<int, EventBudget> eventBudgets;

    for (int i = 0; i < count; i++) {
        int eventId = items[i].getEventId();
        if (!eventBudgets.contains(eventId)) {
            eventBudgets[eventId] = EventBudget(eventId, "Event " + std::to_string(eventId));
        }
        eventBudgets[eventId].addItem(items[i]);
    }

    for (QMap<int, EventBudget>::iterator it = eventBudgets.begin(); it != eventBudgets.end(); ++it) {
        manager.addEvent(it.value());
        if (eventCount < 64) {
            eventIds[eventCount++] = it.key();
        }
    }

    delete[] items;
}

void BudgetView::rebuildTable() {
    budgetTable->setSortingEnabled(false);
    budgetTable->setRowCount(0);

    int selectedEventIndex = eventFilter->currentIndex(); // 0 = All Events, 1+ = specific
    QString typeFilterText = typeFilter->currentText();

    EventBudget allEvts[64];
    int total = manager.getAllEvents(allEvts, 64);

    for (int e = 0; e < total; e++) {
        // Apply event filter
        if (selectedEventIndex > 0) {
            // combo index 1 maps to allEvts[0], etc.
            if (e != selectedEventIndex - 1) continue;
        }

        QString eventName = QString::fromStdString(allEvts[e].getEventName());
        int size = allEvts[e].getSize();

        for (int i = 0; i < size; i++) {
            BudgetItem item = allEvts[e].getItem(i);

            // Apply type filter
            if (typeFilterText != "All Types") {
                if (QString::fromStdString(item.getType()) != typeFilterText) continue;
            }

            int row = budgetTable->rowCount();
            budgetTable->insertRow(row);

            QTableWidgetItem *nameCell = new QTableWidgetItem(eventName);
            // Store itemId and eventId for delete
            nameCell->setData(Qt::UserRole,     item.getID());
            nameCell->setData(Qt::UserRole + 1, allEvts[e].getEventId());

            budgetTable->setItem(row, 0, nameCell);
            budgetTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(item.getType())));
            budgetTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(item.getCategory())));
            budgetTable->setItem(row, 3, new QTableWidgetItem(QString::number(item.getAmount(), 'f', 2)));
            budgetTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(item.getDate())));
            budgetTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(item.getStatus())));
        }
    }

    budgetTable->setSortingEnabled(true);
}

void BudgetView::updateSummary() {
    double totalIncome  = 0.0;
    double totalExpense = 0.0;

    int selectedEventIndex = eventFilter->currentIndex();

    EventBudget allEvts[64];
    int total = manager.getAllEvents(allEvts, 64);

    for (int e = 0; e < total; e++) {
        if (selectedEventIndex > 0 && e != selectedEventIndex - 1) continue;
        totalIncome  += allEvts[e].getTotalIncome();
        totalExpense += allEvts[e].getTotalExpense();
    }

    double profitLoss = totalIncome - totalExpense;

    incomeValue->setText(QString("$%1").arg(totalIncome,  0, 'f', 2));
    expenseValue->setText(QString("$%1").arg(totalExpense, 0, 'f', 2));
    profitValue->setText(QString("$%1").arg(profitLoss,   0, 'f', 2));
}
