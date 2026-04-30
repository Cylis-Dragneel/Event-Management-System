#ifndef VIEWS_DIALOGS_BUDGETITEMDIALOG_H
#define VIEWS_DIALOGS_BUDGETITEMDIALOG_H

#include <QDialog>
#include <QStringList>

class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QDoubleSpinBox;

class BudgetItemDialog : public QDialog {
public:
    explicit BudgetItemDialog(const QString &itemType, QWidget *parent = nullptr);

    void setEvents(const QStringList &eventNames);

    int     getEventIndex() const;
    QString getType()       const;
    QString getCategory()   const;
    double  getAmount()     const;
    QString getDate()       const;
    QString getStatus()     const;

private:
    QComboBox       *eventEdit;
    QComboBox       *typeEdit;
    QComboBox       *categoryEdit;
    QDoubleSpinBox  *amountEdit;
    QDateEdit       *dateEdit;
    QComboBox       *paymentStatusEdit;
    QDialogButtonBox *buttonBox;
};

#endif
