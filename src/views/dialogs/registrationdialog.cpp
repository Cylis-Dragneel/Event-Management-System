#include "registrationdialog.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>

namespace views {
namespace dialogs {

RegistrationDialog::RegistrationDialog(QWidget *parent)
    : QDialog(parent),
      attendeeNameEdit(new QLineEdit(this)),
      attendeeEmailEdit(new QLineEdit(this)),
      eventEdit(new QLineEdit(this)),
      registrationDateEdit(new QDateTimeEdit(this)),
      statusEdit(new QComboBox(this)),
      paymentStatusEdit(new QComboBox(this)),
      notesEdit(new QPlainTextEdit(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Registration");
    resize(500, 420);

    auto *layout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    attendeeNameEdit->setPlaceholderText("Attendee full name");
    formLayout->addRow("Attendee", attendeeNameEdit);

    attendeeEmailEdit->setPlaceholderText("Email");
    formLayout->addRow("Email", attendeeEmailEdit);

    eventEdit->setPlaceholderText("Event name or id");
    formLayout->addRow("Event", eventEdit);

    registrationDateEdit->setCalendarPopup(true);
    registrationDateEdit->setDateTime(QDateTime::currentDateTime());
    formLayout->addRow("Registration Date", registrationDateEdit);

    statusEdit->addItems({"Pending", "Confirmed", "Cancelled", "Waitlisted"});
    formLayout->addRow("Status", statusEdit);

    paymentStatusEdit->addItems({"Pending", "Paid", "Failed", "Refunded"});
    formLayout->addRow("Payment", paymentStatusEdit);

    notesEdit->setPlaceholderText("Notes");
    notesEdit->setMinimumHeight(80);
    formLayout->addRow("Notes", notesEdit);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

}  // namespace dialogs
}  // namespace views
