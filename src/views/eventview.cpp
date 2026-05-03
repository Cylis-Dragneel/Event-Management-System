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

#include "../models/database.h"
#include "../models/user.h"

EventView::EventView(Database *db, bool organizerMode, User *user, QWidget *parent)
    : QWidget(parent),
      database(db),
      currentUser(user),
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

    eventsTable->setColumnCount(9);
    eventsTable->setHorizontalHeaderLabels(
        {"Name", "Date", "Duration", "Type", "Capacity", "Venue", "Cost", "Status", "Registrations"});
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
    registerButton = new QPushButton("Register", this);
    actionsLayout->addWidget(registerButton);
    mainLayout->addLayout(actionsLayout);

    addButton->setEnabled(isOrganizerMode);
    editButton->setEnabled(isOrganizerMode);
    deleteButton->setEnabled(isOrganizerMode);
    registerButton->setEnabled(!isOrganizerMode);

    loadFromDatabase();
    rebuildTable();

    QObject::connect(refreshButton, &QPushButton::clicked, this, [this]() {
        reloadFromDatabase();
        rebuildTable();
    });

    QObject::connect(addButton, &QPushButton::clicked, this, [this]() {
        EventDialog dialog(this);
        if (dialog.exec() != QDialog::Accepted) return;
        if (dialog.getName().isEmpty()) {
            QMessageBox::warning(this, "Add Event", "Event name cannot be empty.");
            return;
        }
        try {
            Event newEvent(dialog.getName().toStdString(),
                           dialog.getDescription().toStdString(),
                           dialog.getDate().toStdString(),
                           dialog.getTime().toStdString(),
                           dialog.getDuration(),
                           dialog.getCapacity(),
                           dialog.getType());
            if (dialog.getVenueId() > 0)
                newEvent.setVenueId(dialog.getVenueId());
            newEvent.setCost(dialog.getCost());
            appendEvent(newEvent);
            rebuildTable();
        } catch (const std::invalid_argument &e) {
            QMessageBox::warning(this, "Add Event", QString::fromStdString(e.what()));
        }
    });

    QObject::connect(editButton, &QPushButton::clicked, this, [this]() {
        int row = eventsTable->currentRow();
        if (row < 0) {
            QMessageBox::information(this, "Edit Event", "Select an event row first.");
            return;
        }
        int idx = eventsTable->item(row, 0)->data(Qt::UserRole).toInt();
        EventDialog dialog(this);
        dialog.populate(*(events + idx));
        if (dialog.exec() != QDialog::Accepted) return;
        try {
            Event &e = *(events + idx);
            e.setName(dialog.getName().toStdString());
            e.setDescription(dialog.getDescription().toStdString());
            e.setDate(dialog.getDate().toStdString());
            e.setTime(dialog.getTime().toStdString());
            e.setDuration(dialog.getDuration());
            e.setCapacity(dialog.getCapacity());
            e.setType(dialog.getType());
            e.setCost(dialog.getCost());
            if (dialog.getVenueId() > 0)
                e.setVenueId(dialog.getVenueId());
            e.changeStatus(dialog.getStatus());
            if (database) {
                database->updateEvent(e.getEventId(), e);
            }
            rebuildTable();
        } catch (const std::invalid_argument &e2) {
            QMessageBox::warning(this, "Edit Event", QString::fromStdString(e2.what()));
        }
    });

    QObject::connect(deleteButton, &QPushButton::clicked, this, [this]() {
        int row = eventsTable->currentRow();
        if (row < 0) {
            QMessageBox::information(this, "Delete Event", "Select an event row first.");
            return;
        }
        int idx = eventsTable->item(row, 0)->data(Qt::UserRole).toInt();
        deleteEvent(idx);
        rebuildTable();
    });

    QObject::connect(registerButton, &QPushButton::clicked, this, [this]() {
        registerForEvent();
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

    if (database) {
        database->addEvent(event);
    }
}

void EventView::loadFromDatabase() {
    if (!database) return;

    int count = 0;
    Event *allEvents = nullptr;
    try {
        allEvents = database->getAllEvents(count);
    } catch (const std::exception& e) {
        qDebug() << "Error loading events:" << e.what();
        return;
    }

    for (int i = 0; i < count; i++) {
        try {
            ensureEventCapacity(eventCount + 1);
            Event *tail = events + eventCount;
            *tail = allEvents[i];
            ++eventCount;
        } catch (const std::exception& e) {
            qDebug() << "Skipping invalid event:" << e.what();
        }
    }

    delete[] allEvents;
}

void EventView::reloadFromDatabase() {
    delete[] events;
    events = nullptr;
    eventCount = 0;
    eventCapacity = 0;
    loadFromDatabase();
}

void EventView::deleteEvent(int index) {
    Event &e = *(events + index);
    if (database) {
        database->deleteEvent(e.getEventId());
    }

    for (int i = index; i < eventCount - 1; i++) {
        *(events + i) = *(events + i + 1);
    }
    --eventCount;
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
        const QString costText = QString("$%1").arg(current->getCost(), 0, 'f', 2);

        const int arrayIndex = eventCount - remaining;
        QTableWidgetItem *nameCell = new QTableWidgetItem(QString::fromStdString(current->getName()));
        nameCell->setData(Qt::UserRole, arrayIndex);
        eventsTable->setItem(row, 0, nameCell);
        eventsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(current->getDate() + " " + current->getTime())));
        eventsTable->setItem(row, 2, new QTableWidgetItem(durationText));
        eventsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(current->getTypeText())));
        eventsTable->setItem(row, 4, new QTableWidgetItem(capacityText));
        eventsTable->setItem(row, 5, new QTableWidgetItem(venueText));
        eventsTable->setItem(row, 6, new QTableWidgetItem(costText));
        eventsTable->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(current->getStatusText())));
        eventsTable->setItem(row, 8, new QTableWidgetItem("0"));

        ++current;
        --remaining;
    }
}

void EventView::registerForEvent() {
    int row = eventsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Register", "Select an event to register for.");
        return;
    }

    int idx = eventsTable->item(row, 0)->data(Qt::UserRole).toInt();
    Event &event = *(events + idx);

    if (event.getStatus() != 1) {
        QMessageBox::warning(this, "Register", "Cannot register for this event. It may not be published.");
        return;
    }

    QString today = QDate::currentDate().toString("dd-MM-yyyy");

    try {
        if (database && currentUser) {
            double eventCost = event.getCost();

            Registration newReg(999999, event.getEventId(), currentUser->getUserId(), 1, 1,
                               today.toStdString(), 0.0, eventCost, "",
                               currentUser->getUsername(), currentUser->getEmail());

            if (database->addRegistration(newReg)) {
                QMessageBox::information(this, "Register", 
                    "Successfully registered for the event!\n\n" +
                    QString("Event: %1\n").arg(QString::fromStdString(event.getName())) +
                    QString("Total Amount: $%1\n").arg(eventCost, 0, 'f', 2) +
                    QString("Status: Pending\n\n") +
                    QString("You can make payments from the 'My Registrations' tab."));
            } else {
                QMessageBox::warning(this, "Register", "Failed to register. Please try again.");
            }
        } else {
            QMessageBox::warning(this, "Register", "Not logged in properly.");
        }
    } catch (const std::invalid_argument &e) {
        QMessageBox::warning(this, "Register Error", QString::fromStdString(e.what()));
    }
}


