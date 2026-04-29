#include "eventview.h"

#include "dialogs/eventdialog.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace views {

EventView::EventView(bool organizerMode, QWidget *parent)
    : QWidget(parent),
      isOrganizerMode(organizerMode),
      searchEdit(new QLineEdit(this)),
      typeFilter(new QComboBox(this)),
      statusFilter(new QComboBox(this)),
      fromDate(new QDateEdit(this)),
      toDate(new QDateEdit(this)),
      eventsTable(new QTableWidget(this)),
      addButton(new QPushButton("Create Event", this)),
      editButton(new QPushButton("Edit Event", this)),
      deleteButton(new QPushButton("Delete Event", this)),
      refreshButton(new QPushButton("Refresh", this)) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *filtersLayout = new QFormLayout();
    searchEdit->setPlaceholderText("Search by event name or description");
    filtersLayout->addRow("Search", searchEdit);

    typeFilter->addItems({"All Types", "Conference", "Workshop", "Concert", "Wedding", "Corporate", "Social"});
    statusFilter->addItems({"All Statuses", "Draft", "Published", "Completed", "Cancelled"});
    filtersLayout->addRow("Type", typeFilter);
    filtersLayout->addRow("Status", statusFilter);

    fromDate->setCalendarPopup(true);
    toDate->setCalendarPopup(true);
    fromDate->setDate(QDate::currentDate().addMonths(-1));
    toDate->setDate(QDate::currentDate().addMonths(1));
    filtersLayout->addRow("From", fromDate);
    filtersLayout->addRow("To", toDate);

    mainLayout->addLayout(filtersLayout);

    eventsTable->setColumnCount(8);
    eventsTable->setHorizontalHeaderLabels(
        {"Name", "Date", "Duration", "Type", "Capacity", "Venue", "Status", "Registrations"});
    eventsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    eventsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    eventsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    eventsTable->setSortingEnabled(true);
    mainLayout->addWidget(eventsTable);

    auto *actionsLayout = new QHBoxLayout();
    actionsLayout->addWidget(refreshButton);
    actionsLayout->addStretch();
    actionsLayout->addWidget(addButton);
    actionsLayout->addWidget(editButton);
    actionsLayout->addWidget(deleteButton);
    mainLayout->addLayout(actionsLayout);

    addButton->setEnabled(isOrganizerMode);
    editButton->setEnabled(isOrganizerMode);
    deleteButton->setEnabled(isOrganizerMode);

    QObject::connect(addButton, &QPushButton::clicked, this, [this]() {
        dialogs::EventDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(editButton, &QPushButton::clicked, this, [this]() {
        if (eventsTable->currentRow() < 0) {
            QMessageBox::information(this, "Edit Event", "Select an event row first.");
            return;
        }
        dialogs::EventDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(deleteButton, &QPushButton::clicked, this, [this]() {
        if (eventsTable->currentRow() < 0) {
            QMessageBox::information(this, "Delete Event", "Select an event row first.");
            return;
        }
        QMessageBox::information(this, "Delete Event", "Delete flow will be connected to controller logic.");
    });
}

}  // namespace views
