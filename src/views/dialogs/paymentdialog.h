#ifndef VIEWS_DIALOGS_PAYMENTDIALOG_H
#define VIEWS_DIALOGS_PAYMENTDIALOG_H

#include <QDialog>

class QDoubleSpinBox;
class QDialogButtonBox;
class QLabel;

class PaymentDialog : public QDialog {
public:
    explicit PaymentDialog(double totalAmount, double amountPaid, QWidget *parent = nullptr);

    double getPaymentAmount() const;

private:
    QDoubleSpinBox *paymentEdit;
    QLabel *balanceLabel;
};

#endif
