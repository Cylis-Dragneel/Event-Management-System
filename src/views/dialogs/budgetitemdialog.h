#ifndef VIEWS_DIALOGS_BUDGETITEMDIALOG_H
#define VIEWS_DIALOGS_BUDGETITEMDIALOG_H

#include <QDialog>

class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QDoubleSpinBox;
class QLineEdit;

namespace views {
namespace dialogs {

class BudgetItemDialog : public QDialog {
public:
    explicit BudgetItemDialog(const QString &itemType, QWidget *parent = nullptr);

private:
    QComboBox *eventEdit;
    QComboBox *typeEdit;
    QLineEdit *categoryEdit;
    QLineEdit *descriptionEdit;
    QDoubleSpinBox *amountEdit;
    QDateEdit *dateEdit;
    QComboBox *paymentStatusEdit;
    QLineEdit *vendorEdit;
    QDialogButtonBox *buttonBox;
};

}  // namespace dialogs
}  // namespace views

#endif
