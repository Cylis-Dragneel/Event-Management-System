#include "budgetview.h"

#include "dialogs/budgetitemdialog.h"
#include "../models/database.h"
#include "../models/event.h"

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
      refreshButton(new QPushButton("Refresh", this)) {

    if (database) {
        int count = 0;
        Event *events = database->getAllEvents(count);
        for (int i = 0; i < count; i++) {
            eventNames.insert(events[i].getEventId(), QString::fromStdString(events[i].getName()));
        }
        delete[] events;
    }

    auto *mainLayout = new QVBoxLayout(this);

    // --- Filters ---
    auto *filtersLayout = new QHBoxLayout();
    eventFilter->addItem("All Events", -1);
    for (QMap<int, QString>::const_iterator it = eventNames.begin(); it != eventNames.end(); ++it) {
        eventFilter->addItem(it.value(), it.key());
    }
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
        QList<int> ids;
        for (QMap<int, QString>::const_iterator it = eventNames.begin(); it != eventNames.end(); ++it) {
            names.append(it.value());
            ids.append(it.key());
        }
        dialog.setEvents(names, ids);

        if (dialog.exec() == QDialog::Accepted) {
            int targetEventId = dialog.getEventId();
            if (targetEventId <= 0) return;

            BudgetItem item(targetEventId,
                            dialog.getType().toStdString(),
                            dialog.getCategory().toStdString(),
                            dialog.getAmount(),
                            dialog.getDate().toStdString(),
                            dialog.getStatus().toStdString());

            if (database) {
                database->addBudgetItem(item);
            }

            if (manager.findEventById(targetEventId) != -1) {
                manager.deleteEvent(targetEventId);
            }
            EventBudget existing(targetEventId, eventNames.value(targetEventId).toStdString());
            if (database) {
                int count = 0;
                BudgetItem *items = database->getAllBudgetItems(count);
                for (int i = 0; i < count; i++) {
                    if (items[i].getEventId() == targetEventId) {
                        existing.addItem(items[i]);
                    }
                }
                delete[] items;
            }
            manager.addEvent(existing);
            rebuildTable();
            updateSummary();
        }
    });

    QObject::connect(addExpenseButton, &QPushButton::clicked, this, [this]() {
        BudgetItemDialog dialog("expense", this);
        QStringList names;
        QList<int> ids;
        for (QMap<int, QString>::const_iterator it = eventNames.begin(); it != eventNames.end(); ++it) {
            names.append(it.value());
            ids.append(it.key());
        }
        dialog.setEvents(names, ids);

        if (dialog.exec() == QDialog::Accepted) {
            int targetEventId = dialog.getEventId();
            if (targetEventId <= 0) return;

            BudgetItem item(targetEventId,
                            dialog.getType().toStdString(),
                            dialog.getCategory().toStdString(),
                            dialog.getAmount(),
                            dialog.getDate().toStdString(),
                            dialog.getStatus().toStdString());

            if (database) {
                database->addBudgetItem(item);
            }

            if (manager.findEventById(targetEventId) != -1) {
                manager.deleteEvent(targetEventId);
            }
            EventBudget existing(targetEventId, eventNames.value(targetEventId).toStdString());
            if (database) {
                int count = 0;
                BudgetItem *items = database->getAllBudgetItems(count);
                for (int i = 0; i < count; i++) {
                    if (items[i].getEventId() == targetEventId) {
                        existing.addItem(items[i]);
                    }
                }
                delete[] items;
            }
            manager.addEvent(existing);
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

        QTableWidgetItem *cell = budgetTable->item(row, 0);
        if (!cell) return;
        int itemId  = cell->data(Qt::UserRole).toInt();
        int evtId   = cell->data(Qt::UserRole + 1).toInt();

        if (database) {
            database->deleteBudgetItem(itemId);
        }

        manager.deleteEvent(evtId);
        EventBudget existing(evtId, eventNames.value(evtId).toStdString());
        if (database) {
            int count = 0;
            BudgetItem *items = database->getAllBudgetItems(count);
            for (int i = 0; i < count; i++) {
                if (items[i].getEventId() == evtId) {
                    existing.addItem(items[i]);
                }
            }
            delete[] items;
        }
        manager.addEvent(existing);
        rebuildTable();
        updateSummary();
    });
}

void BudgetView::loadFromDatabase() {
    if (!database) {
        return;
    }

    manager.clearAll();

    int count = 0;
    BudgetItem *items = database->getAllBudgetItems(count);
    if (!items) {
        return;
    }

    for (int i = 0; i < count; i++) {
        int eventId = items[i].getEventId();
        if (manager.findEventById(eventId) == -1) {
            QString name = eventNames.contains(eventId) ? eventNames.value(eventId) : "Event " + QString::number(eventId);
            manager.addEvent(EventBudget(eventId, name.toStdString()));
        }
    }

    EventBudget allEvts[64];
    int total = manager.getAllEvents(allEvts, 64);
    for (int i = 0; i < count; i++) {
        for (int e = 0; e < total; e++) {
            if (allEvts[e].getEventId() == items[i].getEventId()) {
                EventBudget updated(allEvts[e]);
                updated.addItem(items[i]);
                manager.deleteEvent(items[i].getEventId());
                manager.addEvent(updated);
                break;
            }
        }
    }

    delete[] items;
}

void BudgetView::rebuildTable() {
    budgetTable->setSortingEnabled(false);
    budgetTable->setRowCount(0);

    int selectedEventId = eventFilter->currentData().toInt();
    QString typeFilterText = typeFilter->currentText();

    EventBudget allEvts[64];
    int total = manager.getAllEvents(allEvts, 64);

    for (int e = 0; e < total; e++) {
        if (selectedEventId > 0 && allEvts[e].getEventId() != selectedEventId) continue;

        QString eventName = eventNames.contains(allEvts[e].getEventId())
            ? eventNames.value(allEvts[e].getEventId())
            : QString::fromStdString(allEvts[e].getEventName());
        int size = allEvts[e].getSize();

        for (int i = 0; i < size; i++) {
            BudgetItem item = allEvts[e].getItem(i);

            if (typeFilterText != "All Types") {
                if (QString::fromStdString(item.getType()) != typeFilterText) continue;
            }

            int row = budgetTable->rowCount();
            budgetTable->insertRow(row);

            QTableWidgetItem *nameCell = new QTableWidgetItem(eventName);
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

    int selectedEventId = eventFilter->currentData().toInt();

    EventBudget allEvts[64];
    int total = manager.getAllEvents(allEvts, 64);

    for (int e = 0; e < total; e++) {
        if (selectedEventId > 0 && allEvts[e].getEventId() != selectedEventId) continue;
        totalIncome  += allEvts[e].getTotalIncome();
        totalExpense += allEvts[e].getTotalExpense();
    }

    double profitLoss = totalIncome - totalExpense;

    incomeValue->setText(QString("$%1").arg(totalIncome,  0, 'f', 2));
    expenseValue->setText(QString("$%1").arg(totalExpense, 0, 'f', 2));
    profitValue->setText(QString("$%1").arg(profitLoss,   0, 'f', 2));
}
