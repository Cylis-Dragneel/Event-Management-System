#include "budgetitemdialog.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

BudgetItemDialog::BudgetItemDialog(const QString &itemType, QWidget *parent)
    : QDialog(parent),
      eventEdit(new QComboBox(this)),
      typeEdit(new QComboBox(this)),
      categoryEdit(new QLineEdit(this)),
      descriptionEdit(new QLineEdit(this)),
      amountEdit(new QDoubleSpinBox(this)),
      dateEdit(new QDateEdit(this)),
      paymentStatusEdit(new QComboBox(this)),
      vendorEdit(new QLineEdit(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Budget Item");
    resize(500, 420);

    auto *layout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    eventEdit->addItems({"Select Event"});
    formLayout->addRow("Event", eventEdit);

    typeEdit->addItems({"Income", "Expense"});
    typeEdit->setCurrentText(itemType);
    formLayout->addRow("Type", typeEdit);

    categoryEdit->setPlaceholderText("Category");
    formLayout->addRow("Category", categoryEdit);

    descriptionEdit->setPlaceholderText("Description");
    formLayout->addRow("Description", descriptionEdit);

    amountEdit->setRange(0.01, 100000000.0);
    amountEdit->setDecimals(2);
    amountEdit->setPrefix("$");
    formLayout->addRow("Amount", amountEdit);

    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());
    formLayout->addRow("Date", dateEdit);

    paymentStatusEdit->addItems({"Pending", "Paid", "Failed", "Refunded"});
    formLayout->addRow("Payment Status", paymentStatusEdit);

    vendorEdit->setPlaceholderText("Vendor or payer");
    formLayout->addRow("Vendor", vendorEdit);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
