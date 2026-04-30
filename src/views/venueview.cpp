#include "venueview.h"

#include "dialogs/venuedialog.h"

#include <QComboBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

VenueView::VenueView(bool organizerMode, QWidget *parent)
    : QWidget(parent),
      isOrganizerMode(organizerMode),
      searchEdit(new QLineEdit(this)),
      capacityFilter(new QSpinBox(this)),
      amenityFilter(new QComboBox(this)),
      venuesTable(new QTableWidget(this)),
      addButton(new QPushButton("Add Venue", this)),
      editButton(new QPushButton("Edit Venue", this)),
      deleteButton(new QPushButton("Delete Venue", this)),
      refreshButton(new QPushButton("Refresh", this)),
      venues(nullptr),
      venueCount(0),
      venueCapacity(0) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *filtersLayout = new QFormLayout();
    searchEdit->setPlaceholderText("Search by venue name or address");
    filtersLayout->addRow("Search", searchEdit);

    capacityFilter->setRange(0, 1000);
    capacityFilter->setSpecialValueText("Any");
    filtersLayout->addRow("Minimum Capacity", capacityFilter);

    amenityFilter->addItems({"Any Amenity", "WiFi", "Parking", "Catering", "AV Equipment"});
    filtersLayout->addRow("Amenity", amenityFilter);
    mainLayout->addLayout(filtersLayout);

    venuesTable->setColumnCount(6);
    venuesTable->setHorizontalHeaderLabels(
        {"Name", "Address", "Capacity", "Amenities", "Phone", "Email"});
    venuesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    venuesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    venuesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    venuesTable->setSortingEnabled(true);
    mainLayout->addWidget(venuesTable);

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

    seedVenues();
    rebuildTable();

    QObject::connect(refreshButton, &QPushButton::clicked, this, [this]() {
        rebuildTable();
    });

    QObject::connect(addButton, &QPushButton::clicked, this, [this]() {
        VenueDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(editButton, &QPushButton::clicked, this, [this]() {
        if (venuesTable->currentRow() < 0) {
            QMessageBox::information(this, "Edit Venue", "Select a venue row first.");
            return;
        }
        VenueDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(deleteButton, &QPushButton::clicked, this, [this]() {
        if (venuesTable->currentRow() < 0) {
            QMessageBox::information(this, "Delete Venue", "Select a venue row first.");
            return;
        }
        QMessageBox::information(this, "Delete Venue", "Delete flow will be connected to controller logic.");
    });
}

VenueView::~VenueView() {
    delete[] venues;
}

void VenueView::ensureVenueCapacity(int requiredCount) {
    if (requiredCount <= venueCapacity) {
        return;
    }

    int nextCapacity = venueCapacity > 0 ? venueCapacity : 1;
    while (nextCapacity < requiredCount) {
        nextCapacity *= 2;
    }

    Venue *nextVenues = new Venue[nextCapacity];
    Venue *source = venues;
    Venue *target = nextVenues;
    int remaining = venueCount;
    while (remaining > 0) {
        *target = *source;
        ++target;
        ++source;
        --remaining;
    }

    delete[] venues;
    venues = nextVenues;
    venueCapacity = nextCapacity;
}

void VenueView::appendVenue(const Venue &venue) {
    ensureVenueCapacity(venueCount + 1);
    Venue *tail = venues + venueCount;
    *tail = venue;
    ++venueCount;
}

void VenueView::seedVenues() {
    appendVenue(Venue("Tech Arena", "Main Boulevard, City Center", 350, "0300-1234567", "arena@events.com", true, true, false, true));
    appendVenue(Venue("Community Hall", "Block A, Riverside", 120, "042-12345678", "hall@events.com", true, false, true, false));
    appendVenue(Venue("Grand Expo", "Industrial Zone, Sector 9", 900, "0312-7654321", "expo@events.com", true, true, true, true));
}

void VenueView::rebuildTable() {
    venuesTable->setRowCount(0);

    Venue *current = venues;
    int remaining = venueCount;
    while (remaining > 0) {
        const int row = venuesTable->rowCount();
        venuesTable->insertRow(row);

        venuesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(current->getName())));
        venuesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(current->getAddress())));
        venuesTable->setItem(row, 2, new QTableWidgetItem(QString::number(current->getCapacity())));
        venuesTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(current->getAmenitiesList())));
        venuesTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(current->getContactNumber())));
        venuesTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(current->getContactEmail())));

        ++current;
        --remaining;
    }
}

