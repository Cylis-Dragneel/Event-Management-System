#include "eventdialog.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
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
      venueEdit(new QSpinBox(this)),
      statusEdit(new QComboBox(this)),
      costEdit(new QDoubleSpinBox(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Event");
    resize(520, 460);

    auto *layout     = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    nameEdit->setPlaceholderText("Event name");
    formLayout->addRow("Name", nameEdit);

    descriptionEdit->setPlaceholderText("Description");
    descriptionEdit->setMinimumHeight(70);
    formLayout->addRow("Description", descriptionEdit);

    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("dd-MM-yyyy HH:mm");
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    formLayout->addRow("Date / Time", dateTimeEdit);

    durationEdit->setRange(30, 500);
    durationEdit->setValue(60);
    durationEdit->setSuffix(" min");
    formLayout->addRow("Duration", durationEdit);

    typeEdit->addItems({"Conference", "Workshop", "Concert", "Wedding", "Corporate", "Social"});
    formLayout->addRow("Type", typeEdit);

    capacityEdit->setRange(10, 500);
    capacityEdit->setValue(100);
    formLayout->addRow("Capacity", capacityEdit);

    venueEdit->setRange(0, 99999);
    venueEdit->setValue(0);
    venueEdit->setSpecialValueText("Unassigned");
    formLayout->addRow("Venue ID", venueEdit);

    statusEdit->addItems({"Draft", "Published", "Completed", "Cancelled"});
    formLayout->addRow("Status", statusEdit);

    costEdit->setRange(0, 10000);
    costEdit->setValue(0);
    costEdit->setPrefix("$ ");
    costEdit->setDecimals(2);
    formLayout->addRow("Registration Fee", costEdit);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void EventDialog::populate(const Event &event) {
    nameEdit->setText(QString::fromStdString(event.getName()));
    descriptionEdit->setPlainText(QString::fromStdString(event.getDescription()));

    QString dtStr = QString::fromStdString(event.getDate() + " " + event.getTime());
    dateTimeEdit->setDateTime(QDateTime::fromString(dtStr, "dd-MM-yyyy HH:mm"));

    durationEdit->setValue(event.getDuration());
    typeEdit->setCurrentIndex(event.getType());
    capacityEdit->setValue(event.getCapacity());
    venueEdit->setValue(event.hasVenue() ? event.getVenueId() : 0);
    statusEdit->setCurrentIndex(event.getStatus());
    costEdit->setValue(event.getCost());
}

QString EventDialog::getName() const {
    return nameEdit->text().trimmed();
}

QString EventDialog::getDescription() const {
    return descriptionEdit->toPlainText().trimmed();
}

QString EventDialog::getDate() const {
    return dateTimeEdit->dateTime().toString("dd-MM-yyyy");
}

QString EventDialog::getTime() const {
    return dateTimeEdit->dateTime().toString("HH:mm");
}

int EventDialog::getDuration() const {
    return durationEdit->value();
}

int EventDialog::getType() const {
    return typeEdit->currentIndex();
}

int EventDialog::getCapacity() const {
    return capacityEdit->value();
}

int EventDialog::getVenueId() const {
    return venueEdit->value();
}

int EventDialog::getStatus() const {
    return statusEdit->currentIndex();
}

double EventDialog::getCost() const {
    return costEdit->value();
}
