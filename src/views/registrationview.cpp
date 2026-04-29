#include "registrationview.h"

#include "dialogs/registrationdialog.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace views {

RegistrationView::RegistrationView(bool organizerMode, QWidget *parent)
    : QWidget(parent),
      isOrganizerMode(organizerMode),
      searchEdit(new QLineEdit(this)),
      statusFilter(new QComboBox(this)),
      paymentFilter(new QComboBox(this)),
      fromDate(new QDateEdit(this)),
      toDate(new QDateEdit(this)),
      registrationsTable(new QTableWidget(this)),
      registerButton(new QPushButton("Register Attendee", this)),
      confirmButton(new QPushButton("Confirm", this)),
      cancelButton(new QPushButton("Cancel", this)),
      exportButton(new QPushButton("Export CSV", this)),
      refreshButton(new QPushButton("Refresh", this)) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *filtersLayout = new QFormLayout();
    searchEdit->setPlaceholderText("Search by attendee, event, or email");
    filtersLayout->addRow("Search", searchEdit);

    statusFilter->addItems({"All Statuses", "Pending", "Confirmed", "Cancelled", "Waitlisted"});
    paymentFilter->addItems({"All Payments", "Pending", "Paid", "Failed", "Refunded"});
    filtersLayout->addRow("Registration", statusFilter);
    filtersLayout->addRow("Payment", paymentFilter);

    fromDate->setCalendarPopup(true);
    toDate->setCalendarPopup(true);
    fromDate->setDate(QDate::currentDate().addMonths(-1));
    toDate->setDate(QDate::currentDate().addMonths(1));
    filtersLayout->addRow("From", fromDate);
    filtersLayout->addRow("To", toDate);
    mainLayout->addLayout(filtersLayout);

    registrationsTable->setColumnCount(7);
    registrationsTable->setHorizontalHeaderLabels(
        {"Attendee", "Email", "Event", "Registered", "Status", "Payment", "Notes"});
    registrationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registrationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    registrationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    registrationsTable->setSortingEnabled(true);
    mainLayout->addWidget(registrationsTable);

    auto *actionsLayout = new QHBoxLayout();
    actionsLayout->addWidget(refreshButton);
    actionsLayout->addStretch();
    actionsLayout->addWidget(registerButton);
    actionsLayout->addWidget(confirmButton);
    actionsLayout->addWidget(cancelButton);
    actionsLayout->addWidget(exportButton);
    mainLayout->addLayout(actionsLayout);

    registerButton->setEnabled(isOrganizerMode);
    confirmButton->setEnabled(isOrganizerMode);
    cancelButton->setEnabled(isOrganizerMode);
    exportButton->setEnabled(isOrganizerMode);

    QObject::connect(registerButton, &QPushButton::clicked, this, [this]() {
        dialogs::RegistrationDialog dialog(this);
        dialog.exec();
    });

    QObject::connect(confirmButton, &QPushButton::clicked, this, [this]() {
        if (registrationsTable->currentRow() < 0) {
            QMessageBox::information(this, "Confirm Registration", "Select a registration row first.");
            return;
        }
        QMessageBox::information(this, "Confirm Registration", "Confirm flow will be connected to controller logic.");
    });

    QObject::connect(cancelButton, &QPushButton::clicked, this, [this]() {
        if (registrationsTable->currentRow() < 0) {
            QMessageBox::information(this, "Cancel Registration", "Select a registration row first.");
            return;
        }
        QMessageBox::information(this, "Cancel Registration", "Cancel flow will be connected to controller logic.");
    });

    QObject::connect(exportButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Export CSV", "CSV export flow will be connected to controller logic.");
    });
}

}  // namespace views
