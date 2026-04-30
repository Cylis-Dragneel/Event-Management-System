#include "eventdialog.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

EventDialog::EventDialog(QWidget *parent)
    : QDialog(parent),
      nameEdit(new QLineEdit(this)),
      descriptionEdit(new QPlainTextEdit(this)),
      dateTimeEdit(new QDateTimeEdit(this)),
      durationEdit(new QSpinBox(this)),
      typeEdit(new QComboBox(this)),
      capacityEdit(new QSpinBox(this)),
      venueEdit(new QLineEdit(this)),
      statusEdit(new QComboBox(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Event");
    resize(520, 460);

    auto *layout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    nameEdit->setPlaceholderText("Event name");
    formLayout->addRow("Name", nameEdit);

    descriptionEdit->setPlaceholderText("Description");
    descriptionEdit->setMinimumHeight(90);
    formLayout->addRow("Description", descriptionEdit);

    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    formLayout->addRow("Date/Time", dateTimeEdit);

    durationEdit->setRange(15, 1440);
    durationEdit->setValue(60);
    durationEdit->setSuffix(" min");
    formLayout->addRow("Duration", durationEdit);

    typeEdit->addItems({"Conference", "Workshop", "Concert", "Wedding", "Corporate", "Social"});
    formLayout->addRow("Type", typeEdit);

    capacityEdit->setRange(1, 100000);
    capacityEdit->setValue(100);
    formLayout->addRow("Capacity", capacityEdit);

    venueEdit->setPlaceholderText("Venue name or id");
    formLayout->addRow("Venue", venueEdit);

    statusEdit->addItems({"Draft", "Published", "Completed", "Cancelled"});
    formLayout->addRow("Status", statusEdit);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

