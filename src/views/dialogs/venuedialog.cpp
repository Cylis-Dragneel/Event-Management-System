#include "venuedialog.h"

#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

namespace views {
namespace dialogs {

VenueDialog::VenueDialog(QWidget *parent)
    : QDialog(parent),
      nameEdit(new QLineEdit(this)),
      addressEdit(new QLineEdit(this)),
      capacityEdit(new QSpinBox(this)),
      descriptionEdit(new QPlainTextEdit(this)),
      amenitiesEdit(new QLineEdit(this)),
      hourlyRateEdit(new QDoubleSpinBox(this)),
      contactEdit(new QLineEdit(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Venue");
    resize(520, 450);

    auto *layout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    nameEdit->setPlaceholderText("Venue name");
    formLayout->addRow("Name", nameEdit);

    addressEdit->setPlaceholderText("Address");
    formLayout->addRow("Address", addressEdit);

    capacityEdit->setRange(1, 100000);
    capacityEdit->setValue(100);
    formLayout->addRow("Capacity", capacityEdit);

    descriptionEdit->setPlaceholderText("Description");
    descriptionEdit->setMinimumHeight(80);
    formLayout->addRow("Description", descriptionEdit);

    amenitiesEdit->setPlaceholderText("Comma-separated amenities");
    formLayout->addRow("Amenities", amenitiesEdit);

    hourlyRateEdit->setRange(0.0, 1000000.0);
    hourlyRateEdit->setDecimals(2);
    hourlyRateEdit->setPrefix("$");
    formLayout->addRow("Hourly Rate", hourlyRateEdit);

    contactEdit->setPlaceholderText("Phone or email");
    formLayout->addRow("Contact", contactEdit);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

}  // namespace dialogs
}  // namespace views
