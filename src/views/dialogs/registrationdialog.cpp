#include "registrationdialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>

RegistrationDialog::RegistrationDialog(QWidget *parent)
    : QDialog(parent),
      firstNameEdit(new QLineEdit(this)),
      lastNameEdit(new QLineEdit(this)),
      emailEdit(new QLineEdit(this)),
      phoneEdit(new QLineEdit(this)),
      eventEdit(new QComboBox(this)),
      totalAmountEdit(new QDoubleSpinBox(this)),
      notesEdit(new QPlainTextEdit(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Register Attendee");
    resize(480, 360);

    auto *layout = new QVBoxLayout(this);
    auto *form   = new QFormLayout();

    firstNameEdit->setPlaceholderText("First name");
    form->addRow("First Name", firstNameEdit);

    lastNameEdit->setPlaceholderText("Last name");
    form->addRow("Last Name", lastNameEdit);

    emailEdit->setPlaceholderText("example@domain.com");
    form->addRow("Email", emailEdit);

    phoneEdit->setPlaceholderText("03XX-XXXXXXX  (optional)");
    form->addRow("Phone", phoneEdit);

    form->addRow("Event", eventEdit);

    totalAmountEdit->setRange(0.0, 10000000.0);
    totalAmountEdit->setDecimals(2);
    totalAmountEdit->setPrefix("$");
    form->addRow("Total Amount", totalAmountEdit);

    notesEdit->setPlaceholderText("Notes (optional)");
    notesEdit->setMaximumHeight(70);
    form->addRow("Notes", notesEdit);

    layout->addLayout(form);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void RegistrationDialog::setEvents(const QStringList &eventNames) {
    eventEdit->clear();
    eventEdit->addItems(eventNames);
}

int RegistrationDialog::getEventIndex() const {
    return eventEdit->currentIndex();
}

QString RegistrationDialog::getFirstName() const {
    return firstNameEdit->text().trimmed();
}

QString RegistrationDialog::getLastName() const {
    return lastNameEdit->text().trimmed();
}

QString RegistrationDialog::getEmail() const {
    return emailEdit->text().trimmed();
}

QString RegistrationDialog::getPhone() const {
    return phoneEdit->text().trimmed();
}

double RegistrationDialog::getTotalAmount() const {
    return totalAmountEdit->value();
}

QString RegistrationDialog::getNotes() const {
    return notesEdit->toPlainText().trimmed();
}
