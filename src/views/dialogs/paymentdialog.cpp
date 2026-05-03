#include "paymentdialog.h"

#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>

PaymentDialog::PaymentDialog(double totalAmount, double amountPaid, QWidget *parent)
    : QDialog(parent),
      paymentEdit(new QDoubleSpinBox(this)),
      balanceLabel(new QLabel(this)) {

    setWindowTitle("Make Payment");
    resize(300, 200);

    double balance = totalAmount - amountPaid;

    paymentEdit->setRange(0.01, balance);
    paymentEdit->setDecimals(2);
    paymentEdit->setPrefix("$");
    paymentEdit->setValue(balance);

    balanceLabel->setText(QString("Balance due: $%1").arg(balance, 0, 'f', 2));

    auto *layout = new QVBoxLayout(this);
    auto *form = new QFormLayout();
    form->addRow("Amount to Pay", paymentEdit);
    form->addRow("Balance", balanceLabel);
    layout->addLayout(form);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

double PaymentDialog::getPaymentAmount() const {
    return paymentEdit->value();
}
