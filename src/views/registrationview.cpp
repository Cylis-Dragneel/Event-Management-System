#include "registrationview.h"

#include "dialogs/registrationdialog.h"
#include "../models/CSVExporter.h"

#include <QComboBox>
#include <QDate>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include <stdexcept>
#include <cstring>

RegistrationView::RegistrationView(bool organizerMode, QWidget *parent)
    : QWidget(parent),
      isOrganizerMode(organizerMode),
      searchEdit(new QLineEdit(this)),
      statusFilter(new QComboBox(this)),
      paymentFilter(new QComboBox(this)),
      registrationsTable(new QTableWidget(this)),
      registerButton(new QPushButton("Register Attendee", this)),
      confirmButton(new QPushButton("Confirm", this)),
      cancelButton(new QPushButton("Cancel", this)),
      exportButton(new QPushButton("Export CSV", this)),
      refreshButton(new QPushButton("Refresh", this)),
      regCount(0),
      nextRegId(1) {

    // Hardcoded events matching EventView seed
    eventIds[0] = 1; strcpy(eventNames[0], "Tech Summit 2026");
    eventIds[1] = 2; strcpy(eventNames[1], "Design Workshop");
    eventIds[2] = 3; strcpy(eventNames[2], "Community Concert");

    auto *mainLayout = new QVBoxLayout(this);

    // --- Filters ---
    auto *filtersLayout = new QFormLayout();
    searchEdit->setPlaceholderText("Search by name or email");
    filtersLayout->addRow("Search", searchEdit);
    statusFilter->addItems({"All Statuses", "Pending", "Confirmed", "Cancelled", "Waitlisted"});
    paymentFilter->addItems({"All Payments", "Unpaid", "Paid", "Refunded", "Partial"});
    filtersLayout->addRow("Status", statusFilter);
    filtersLayout->addRow("Payment", paymentFilter);
    mainLayout->addLayout(filtersLayout);

    // --- Table ---
    registrationsTable->setColumnCount(7);
    registrationsTable->setHorizontalHeaderLabels(
        {"Attendee", "Email", "Event", "Date", "Status", "Payment", "Balance"});
    registrationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registrationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    registrationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    registrationsTable->setSortingEnabled(true);
    mainLayout->addWidget(registrationsTable);

    // --- Actions ---
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

    seedData();
    rebuildTable();

    QObject::connect(refreshButton, &QPushButton::clicked, this, [this]() {
        rebuildTable();
    });

    QObject::connect(searchEdit, &QLineEdit::textChanged, this, [this]() {
        rebuildTable();
    });

    QObject::connect(statusFilter, &QComboBox::currentIndexChanged, this, [this](int) {
        rebuildTable();
    });

    QObject::connect(paymentFilter, &QComboBox::currentIndexChanged, this, [this](int) {
        rebuildTable();
    });

    QObject::connect(registerButton, &QPushButton::clicked, this, [this]() {
        RegistrationDialog dialog(this);
        QStringList names;
        for (int i = 0; i < EVENT_COUNT; i++) {
            names << QString(eventNames[i]);
        }
        dialog.setEvents(names);

        if (dialog.exec() != QDialog::Accepted) return;

        int idx = dialog.getEventIndex();
        if (idx < 0 || idx >= EVENT_COUNT) return;

        if (regCount >= 64) {
            QMessageBox::warning(this, "Register", "Maximum registrations reached.");
            return;
        }

        QString firstName = dialog.getFirstName();
        QString lastName  = dialog.getLastName();
        QString email     = dialog.getEmail();
        QString phone     = dialog.getPhone();
        double  total     = dialog.getTotalAmount();
        QString notes     = dialog.getNotes();

        if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty()) {
            QMessageBox::warning(this, "Register", "First name, last name, and email are required.");
            return;
        }

        Attendee newAttendee(firstName.toStdString(),
                             lastName.toStdString(),
                             email.toStdString(),
                             phone.toStdString());

        QString today = QDate::currentDate().toString("dd-MM-yyyy");

        try {
            Registration newReg(nextRegId,
                                eventIds[idx],
                                newAttendee.getAttendeeId(),
                                1,       // Pending
                                1,       // Unpaid
                                today.toStdString(),
                                0.0,
                                total,
                                notes.toStdString());

            attendees[regCount]     = newAttendee;
            registrations[regCount] = newReg;
            regCount++;
            nextRegId++;
            rebuildTable();
        } catch (const invalid_argument &e) {
            QMessageBox::warning(this, "Registration Error", QString::fromStdString(e.what()));
        }
    });

    QObject::connect(confirmButton, &QPushButton::clicked, this, [this]() {
        int row = registrationsTable->currentRow();
        if (row < 0) {
            QMessageBox::information(this, "Confirm", "Select a registration row first.");
            return;
        }
        int idx = registrationsTable->item(row, 0)->data(Qt::UserRole).toInt();
        try {
            registrations[idx].confirmRegistration();
            rebuildTable();
        } catch (const invalid_argument &e) {
            QMessageBox::warning(this, "Confirm Error", QString::fromStdString(e.what()));
        }
    });

    QObject::connect(cancelButton, &QPushButton::clicked, this, [this]() {
        int row = registrationsTable->currentRow();
        if (row < 0) {
            QMessageBox::information(this, "Cancel", "Select a registration row first.");
            return;
        }
        int idx = registrationsTable->item(row, 0)->data(Qt::UserRole).toInt();
        try {
            registrations[idx].cancelRegistration();
            rebuildTable();
        } catch (const invalid_argument &e) {
            QMessageBox::warning(this, "Cancel Error", QString::fromStdString(e.what()));
        }
    });

    QObject::connect(exportButton, &QPushButton::clicked, this, [this]() {
        if (regCount == 0) {
            QMessageBox::information(this, "Export CSV", "No registrations to export.");
            return;
        }
        QString path = QFileDialog::getSaveFileName(this, "Export Attendees", "attendees.csv", "CSV Files (*.csv)");
        if (path.isEmpty()) return;
        bool ok = CSVExporter::exportAttendeesToCSV(attendees, regCount, path.toStdString());
        if (ok) {
            QMessageBox::information(this, "Export CSV", "Exported successfully to:\n" + path);
        } else {
            QMessageBox::warning(this, "Export CSV", "Failed to write file.");
        }
    });
}

void RegistrationView::seedData() {
    // Seed attendees using the non-validating auto-ID constructor
    attendees[0] = Attendee("Ali",    "Hassan", "ali.hassan@email.com",   "0301-1234567");
    attendees[1] = Attendee("Sara",   "Ahmed",  "sara.ahmed@email.com",   "");
    attendees[2] = Attendee("Umar",   "Khan",   "umar.khan@email.com",    "0312-9876543");
    attendees[3] = Attendee("Fatima", "Ali",    "fatima.ali@email.com",   "");

    // Seed registrations
    registrations[0] = Registration(nextRegId++, 1, attendees[0].getAttendeeId(), 2, 2, "10-09-2026", 150.0, 150.0, "VIP seat");
    registrations[1] = Registration(nextRegId++, 1, attendees[1].getAttendeeId(), 1, 1, "12-09-2026",   0.0, 150.0, "");
    registrations[2] = Registration(nextRegId++, 2, attendees[2].getAttendeeId(), 2, 4, "15-08-2026",  50.0, 100.0, "Partial payment");
    registrations[3] = Registration(nextRegId++, 3, attendees[3].getAttendeeId(), 4, 1, "20-09-2026",   0.0, 200.0, "On waitlist");

    regCount = 4;
}

void RegistrationView::rebuildTable() {
    registrationsTable->setSortingEnabled(false);
    registrationsTable->setRowCount(0);

    QString search      = searchEdit->text().trimmed().toLower();
    int statusIdx       = statusFilter->currentIndex();   // 0=All, 1=Pending...
    int paymentIdx      = paymentFilter->currentIndex();  // 0=All, 1=Unpaid...

    for (int i = 0; i < regCount; i++) {
        const Registration &reg = registrations[i];
        const Attendee     &att = attendees[i];

        // Search filter
        if (!search.isEmpty()) {
            QString name  = QString::fromStdString(att.getFullName()).toLower();
            QString email = QString::fromStdString(att.getEmail()).toLower();
            if (!name.contains(search) && !email.contains(search)) continue;
        }

        // Status filter (combo index 1 = Pending = status 1, etc.)
        if (statusIdx > 0 && reg.getRegistrationStatus() != statusIdx) continue;

        // Payment filter (combo index 1 = Unpaid = paymentStatus 1, etc.)
        if (paymentIdx > 0 && reg.getPaymentStatus() != paymentIdx) continue;

        // Event name lookup
        QString eventName = "Unknown";
        for (int e = 0; e < EVENT_COUNT; e++) {
            if (eventIds[e] == reg.getEventId()) {
                eventName = QString(eventNames[e]);
                break;
            }
        }

        double balance = reg.getTotalAmount() - reg.getAmountPaid();

        int row = registrationsTable->rowCount();
        registrationsTable->insertRow(row);

        QTableWidgetItem *nameCell = new QTableWidgetItem(QString::fromStdString(att.getFullName()));
        nameCell->setData(Qt::UserRole, i);  // store array index for action buttons

        registrationsTable->setItem(row, 0, nameCell);
        registrationsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(att.getEmail())));
        registrationsTable->setItem(row, 2, new QTableWidgetItem(eventName));
        registrationsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(reg.getRegistrationDate())));
        registrationsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(reg.getRegistrationStatusText())));
        registrationsTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(reg.getPaymentStatusText())));
        registrationsTable->setItem(row, 6, new QTableWidgetItem(QString("$%1").arg(balance, 0, 'f', 2)));
    }

    registrationsTable->setSortingEnabled(true);
}
