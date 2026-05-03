#include "registrationview.h"

#include "dialogs/paymentdialog.h"
#include "dialogs/registrationdialog.h"
#include "../models/CSVExporter.h"
#include "../models/database.h"

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

RegistrationView::RegistrationView(Database *db, bool organizerMode, User *user, QWidget *parent)
    : QWidget(parent),
      database(db),
      currentUser(user),
      isOrganizerMode(organizerMode),
      searchEdit(new QLineEdit(this)),
      statusFilter(new QComboBox(this)),
      paymentFilter(new QComboBox(this)),
      registrationsTable(new QTableWidget(this)),
      registerButton(new QPushButton("Register Attendee", this)),
      confirmButton(new QPushButton("Confirm", this)),
      cancelButton(new QPushButton("Cancel", this)),
      payButton(new QPushButton("Pay", this)),
      exportButton(new QPushButton("Export CSV", this)),
      refreshButton(new QPushButton("Refresh", this)),
      attendees(new Attendee[64]),
      registrations(new Registration[64]),
      regCount(0),
      regCapacity(64),
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
    actionsLayout->addWidget(payButton);
    actionsLayout->addWidget(exportButton);
    mainLayout->addLayout(actionsLayout);

    registerButton->setEnabled(isOrganizerMode);
    confirmButton->setEnabled(isOrganizerMode);
    cancelButton->setEnabled(!isOrganizerMode);
    payButton->setEnabled(!isOrganizerMode);
    exportButton->setEnabled(isOrganizerMode);

    if (!isOrganizerMode) {
        cancelButton->setText("Unregister");
    }

    loadFromDatabase();
    rebuildTable();

    QObject::connect(refreshButton, &QPushButton::clicked, this, [this]() {
        reloadFromDatabase();
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
            // Save attendee to database first
            if (database) {
                database->addAttendee(newAttendee);
            }

            // Get the attendee ID (assuming it was auto-incremented)
            int attendeeId = newAttendee.getAttendeeId();
            if (attendeeId <= 0 && database) {
                // If attendee ID not set, we need to get it from DB
                int attCount = 0;
                Attendee* allAtts = database->getAllAttendees(attCount);
                if (attCount > 0) {
                    attendeeId = allAtts[attCount - 1].getAttendeeId();
                }
                delete[] allAtts;
            }

            Registration newReg(nextRegId,
                                eventIds[idx],
                                attendeeId,
                                1,       // Pending
                                1,       // Unpaid
                                today.toStdString(),
                                0.0,
                                total,
                                notes.toStdString(),
                                (firstName + " " + lastName).toStdString(),
                                email.toStdString());

            // Save registration to database
            if (database) {
                database->addRegistration(newReg);
                // Get the registration ID from DB
                int regCountDb = 0;
                Registration* allRegs = database->getAllRegistrations(regCountDb);
                if (regCountDb > 0) {
                    newReg.setRegistrationId(allRegs[regCountDb - 1].getRegistrationId());
                    nextRegId = allRegs[regCountDb - 1].getRegistrationId() + 1;
                }
                delete[] allRegs;
            }

            attendees[regCount]     = newAttendee;
            registrations[regCount] = newReg;
            regCount++;
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
        unregister();
    });

    QObject::connect(payButton, &QPushButton::clicked, this, [this]() {
        makePayment();
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

void RegistrationView::loadFromDatabase() {
    if (!database) {
        return;
    }

    int regCountDb = 0;
    Registration *allRegs = nullptr;
    try {
        allRegs = database->getAllRegistrations(regCountDb);
    } catch (const std::exception& e) {
        qDebug() << "Error loading registrations:" << e.what();
        return;
    }

    for (int i = 0; i < regCountDb && regCount < regCapacity; i++) {
        try {
            Registration reg = allRegs[i];
            if (!isOrganizerMode && currentUser) {
                if (reg.getAttendeeId() != currentUser->getUserId()) {
                    continue;
                }
            }
            registrations[regCount] = reg;
            regCount++;
        } catch (const std::exception& e) {
            qDebug() << "Skipping invalid registration:" << e.what();
        }
    }

    delete[] allRegs;
}

void RegistrationView::reloadFromDatabase() {
    Registration *freshRegs = new Registration[regCapacity];
    delete[] registrations;
    registrations = freshRegs;
    regCount = 0;

    loadFromDatabase();
}

void RegistrationView::rebuildTable() {
    registrationsTable->setSortingEnabled(false);
    registrationsTable->setRowCount(0);

    QString search      = searchEdit->text().trimmed().toLower();
    int statusIdx       = statusFilter->currentIndex();   // 0=All, 1=Pending...
    int paymentIdx      = paymentFilter->currentIndex();  // 0=All, 1=Unpaid...

    for (int i = 0; i < regCount; i++) {
        const Registration &reg = registrations[i];

        QString displayName = QString::fromStdString(reg.getAttendeeName());
        QString displayEmail = QString::fromStdString(reg.getAttendeeEmail());

        if (!search.isEmpty()) {
            QString nameLower = displayName.toLower();
            QString emailLower = displayEmail.toLower();
            if (!nameLower.contains(search) && !emailLower.contains(search)) continue;
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

        QTableWidgetItem *nameCell = new QTableWidgetItem(displayName);
        nameCell->setData(Qt::UserRole, i);

        registrationsTable->setItem(row, 0, nameCell);
        registrationsTable->setItem(row, 1, new QTableWidgetItem(displayEmail));
        registrationsTable->setItem(row, 2, new QTableWidgetItem(eventName));
        registrationsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(reg.getRegistrationDate())));
        registrationsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(reg.getRegistrationStatusText())));
        registrationsTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(reg.getPaymentStatusText())));
        registrationsTable->setItem(row, 6, new QTableWidgetItem(QString("$%1").arg(balance, 0, 'f', 2)));
    }

    registrationsTable->setSortingEnabled(true);
}

void RegistrationView::unregister() {
    int row = registrationsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Unregister", "Select a registration to unregister from.");
        return;
    }

    int idx = registrationsTable->item(row, 0)->data(Qt::UserRole).toInt();
    Registration &reg = registrations[idx];

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Unregister",
        "Are you sure you want to unregister from this event?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (database) {
            database->deleteRegistration(reg.getRegistrationId());
        }

        for (int i = idx; i < regCount - 1; i++) {
            registrations[i] = registrations[i + 1];
            attendees[i] = attendees[i + 1];
        }
        regCount--;
        rebuildTable();

        QMessageBox::information(this, "Unregister", "Successfully unregistered from the event.");
    }
}

void RegistrationView::makePayment() {
    int row = registrationsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Make Payment", "Select a registration to make payment.");
        return;
    }

    int idx = registrationsTable->item(row, 0)->data(Qt::UserRole).toInt();
    Registration &reg = registrations[idx];

    if (reg.isFullyPaid()) {
        QMessageBox::information(this, "Make Payment", "This registration is already fully paid.");
        return;
    }

    if (reg.getRegistrationStatus() == 3) {
        QMessageBox::warning(this, "Make Payment", "Cannot pay for a cancelled registration.");
        return;
    }

    PaymentDialog dialog(reg.getTotalAmount(), reg.getAmountPaid(), this);

    if (dialog.exec() != QDialog::Accepted) return;

    double paymentAmount = dialog.getPaymentAmount();

    try {
        reg.makePayment(paymentAmount);

        if (database) {
            database->updateRegistration(reg.getRegistrationId(), reg);
        }

        rebuildTable();
        QMessageBox::information(this, "Payment", "Payment successful! Amount paid: $" + QString::number(paymentAmount, 'f', 2));
    } catch (const invalid_argument &e) {
        QMessageBox::warning(this, "Payment Error", QString::fromStdString(e.what()));
    }
}

RegistrationView::~RegistrationView() {
    delete[] attendees;
    delete[] registrations;
}
