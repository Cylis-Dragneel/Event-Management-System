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
#include <QTableWidgetItem>
#include <QVBoxLayout>

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
      refreshButton(new QPushButton("Refresh", this)),
      events(nullptr),
      eventCount(0),
      eventCapacity(0) {
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

    seedEvents();
    rebuildTable();

    QObject::connect(refreshButton, &QPushButton::clicked, this, [this]() {
        rebuildTable();
    });

    QObject::connect(addButton, &QPushButton::clicked, this, [this]() {
        EventDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(editButton, &QPushButton::clicked, this, [this]() {
        if (eventsTable->currentRow() < 0) {
            QMessageBox::information(this, "Edit Event", "Select an event row first.");
            return;
        }
        EventDialog dialog(this);
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

EventView::~EventView() {
    delete[] events;
}

void EventView::ensureEventCapacity(int requiredCount) {
    if (requiredCount <= eventCapacity) {
        return;
    }

    int nextCapacity = eventCapacity > 0 ? eventCapacity : 1;
    while (nextCapacity < requiredCount) {
        nextCapacity *= 2;
    }

    Event *nextEvents = new Event[nextCapacity];
    Event *source = events;
    Event *target = nextEvents;
    int remaining = eventCount;
    while (remaining > 0) {
        *target = *source;
        ++target;
        ++source;
        --remaining;
    }

    delete[] events;
    events = nextEvents;
    eventCapacity = nextCapacity;
}

void EventView::appendEvent(const Event &event) {
    ensureEventCapacity(eventCount + 1);
    Event *tail = events + eventCount;
    *tail = event;
    ++eventCount;
}

void EventView::seedEvents() {
    appendEvent(Event("Tech Summit 2026", "Annual technology conference", "15-09-2026", "10:00", 180, 300, 0));
    appendEvent(Event("Design Workshop", "Hands-on design sprint", "22-08-2026", "09:30", 120, 60, 1));
    appendEvent(Event("Community Concert", "Outdoor social music event", "05-10-2026", "18:30", 150, 500, 2));

    Event *first = events;
    first->setVenueId(101);

    Event *second = events + 1;
    second->setVenueId(203);
    second->changeStatus(1);

    Event *third = events + 2;
    third->setVenueId(305);
    third->changeStatus(1);
}

void EventView::rebuildTable() {
    eventsTable->setRowCount(0);

    Event *current = events;
    int remaining = eventCount;
    while (remaining > 0) {
        const int row = eventsTable->rowCount();
        eventsTable->insertRow(row);

        const QString durationText = QString::number(current->getDuration()) + " min";
        const QString capacityText = QString::number(current->getCapacity());
        const QString venueText = current->hasVenue() ? QString::number(current->getVenueId()) : QString("Unassigned");

        eventsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(current->getName())));
        eventsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(current->getDate() + " " + current->getTime())));
        eventsTable->setItem(row, 2, new QTableWidgetItem(durationText));
        eventsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(current->getTypeText())));
        eventsTable->setItem(row, 4, new QTableWidgetItem(capacityText));
        eventsTable->setItem(row, 5, new QTableWidgetItem(venueText));
        eventsTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(current->getStatusText())));
        eventsTable->setItem(row, 7, new QTableWidgetItem("0"));

        ++current;
        --remaining;
    }
}

