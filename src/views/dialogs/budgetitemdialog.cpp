#include "budgetitemdialog.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>

#include <QVBoxLayout>

BudgetItemDialog::BudgetItemDialog(const QString &itemType, QWidget *parent)
    : QDialog(parent),
      eventEdit(new QComboBox(this)),
      typeEdit(new QComboBox(this)),
      categoryEdit(new QComboBox(this)),
      amountEdit(new QDoubleSpinBox(this)),
      dateEdit(new QDateEdit(this)),
      paymentStatusEdit(new QComboBox(this)),
      buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)) {
    setWindowTitle("Budget Item");
    resize(420, 320);

    auto *layout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    formLayout->addRow("Event", eventEdit);

    typeEdit->addItems({"income", "expense"});
    typeEdit->setCurrentText(itemType.toLower());
    formLayout->addRow("Type", typeEdit);

    categoryEdit->addItems({"venue", "catering", "marketing", "staff", "equipment", "other"});
    formLayout->addRow("Category", categoryEdit);

    amountEdit->setRange(0.01, 100000000.0);
    amountEdit->setDecimals(2);
    amountEdit->setPrefix("$");
    formLayout->addRow("Amount", amountEdit);

    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());
    formLayout->addRow("Date", dateEdit);

    paymentStatusEdit->addItems({"pending", "paid"});
    formLayout->addRow("Payment Status", paymentStatusEdit);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void BudgetItemDialog::setEvents(const QStringList &eventNames, const QList<int> &eventIds) {
    eventEdit->clear();
    for (int i = 0; i < eventNames.size(); i++) {
        eventEdit->addItem(eventNames[i], eventIds[i]);
    }
}

int BudgetItemDialog::getEventId() const {
    return eventEdit->currentData().toInt();
}

QString BudgetItemDialog::getType() const {
    return typeEdit->currentText();
}

QString BudgetItemDialog::getCategory() const {
    return categoryEdit->currentText();
}

double BudgetItemDialog::getAmount() const {
    return amountEdit->value();
}

QString BudgetItemDialog::getDate() const {
    return dateEdit->date().toString("dd-MM-yyyy");
}

QString BudgetItemDialog::getStatus() const {
    return paymentStatusEdit->currentText();
}
