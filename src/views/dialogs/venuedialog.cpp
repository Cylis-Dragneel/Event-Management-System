#include "venuedialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>

VenueDialog::VenueDialog(QWidget *parent)
    : QDialog(parent),
      nameEdit(new QLineEdit(this)),
      addressEdit(new QLineEdit(this)),
      capacityEdit(new QSpinBox(this)),
      contactNumberEdit(new QLineEdit(this)),
      contactEmailEdit(new QLineEdit(this)),
      wifiCheck(new QCheckBox("WiFi", this)),
      parkingCheck(new QCheckBox("Parking", this)),
      cateringCheck(new QCheckBox("Catering", this)),
      avCheck(new QCheckBox("AV Equipment", this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Venue");
    resize(480, 340);

    auto *layout     = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    nameEdit->setPlaceholderText("Venue name");
    formLayout->addRow("Name", nameEdit);

    addressEdit->setPlaceholderText("Street, City");
    formLayout->addRow("Address", addressEdit);

    capacityEdit->setRange(1, 100000);
    capacityEdit->setValue(100);
    formLayout->addRow("Capacity", capacityEdit);

    contactNumberEdit->setPlaceholderText("03XX-XXXXXXX or 042-XXXXXXXX");
    formLayout->addRow("Phone", contactNumberEdit);

    contactEmailEdit->setPlaceholderText("contact@venue.com");
    formLayout->addRow("Email", contactEmailEdit);

    auto *amenitiesLayout = new QHBoxLayout();
    amenitiesLayout->addWidget(wifiCheck);
    amenitiesLayout->addWidget(parkingCheck);
    amenitiesLayout->addWidget(cateringCheck);
    amenitiesLayout->addWidget(avCheck);
    amenitiesLayout->addStretch();
    formLayout->addRow("Amenities", amenitiesLayout);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void VenueDialog::populate(const Venue &venue) {
    nameEdit->setText(QString::fromStdString(venue.getName()));
    addressEdit->setText(QString::fromStdString(venue.getAddress()));
    capacityEdit->setValue(venue.getCapacity());
    contactNumberEdit->setText(QString::fromStdString(venue.getContactNumber()));
    contactEmailEdit->setText(QString::fromStdString(venue.getContactEmail()));
    wifiCheck->setChecked(venue.getHasWifi());
    parkingCheck->setChecked(venue.getHasParking());
    cateringCheck->setChecked(venue.getHasCatering());
    avCheck->setChecked(venue.getHasAVEquipment());
}

QString VenueDialog::getName() const {
    return nameEdit->text().trimmed();
}

QString VenueDialog::getAddress() const {
    return addressEdit->text().trimmed();
}

int VenueDialog::getCapacity() const {
    return capacityEdit->value();
}

QString VenueDialog::getContactNumber() const {
    return contactNumberEdit->text().trimmed();
}

QString VenueDialog::getContactEmail() const {
    return contactEmailEdit->text().trimmed();
}

bool VenueDialog::getHasWifi() const {
    return wifiCheck->isChecked();
}

bool VenueDialog::getHasParking() const {
    return parkingCheck->isChecked();
}

bool VenueDialog::getHasCatering() const {
    return cateringCheck->isChecked();
}

bool VenueDialog::getHasAVEquipment() const {
    return avCheck->isChecked();
}
