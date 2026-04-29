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
#include <QVBoxLayout>

namespace views {

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
      refreshButton(new QPushButton("Refresh", this)) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *filtersLayout = new QFormLayout();
    searchEdit->setPlaceholderText("Search by venue name or address");
    filtersLayout->addRow("Search", searchEdit);

    capacityFilter->setRange(0, 100000);
    capacityFilter->setSpecialValueText("Any");
    filtersLayout->addRow("Minimum Capacity", capacityFilter);

    amenityFilter->addItems({"Any Amenity", "WiFi", "Parking", "Catering", "AV Equipment", "Accessibility", "Stage", "Tables/Chairs"});
    filtersLayout->addRow("Amenity", amenityFilter);
    mainLayout->addLayout(filtersLayout);

    venuesTable->setColumnCount(6);
    venuesTable->setHorizontalHeaderLabels(
        {"Name", "Address", "Capacity", "Amenities", "Hourly Rate", "Contact"});
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

    QObject::connect(addButton, &QPushButton::clicked, this, [this]() {
        dialogs::VenueDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(editButton, &QPushButton::clicked, this, [this]() {
        if (venuesTable->currentRow() < 0) {
            QMessageBox::information(this, "Edit Venue", "Select a venue row first.");
            return;
        }
        dialogs::VenueDialog dialog(this);
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

}  // namespace views
