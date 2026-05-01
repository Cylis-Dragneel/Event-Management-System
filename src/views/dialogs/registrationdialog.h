#ifndef VIEWS_DIALOGS_REGISTRATIONDIALOG_H
#define VIEWS_DIALOGS_REGISTRATIONDIALOG_H

#include <QDialog>
#include <QStringList>

class QComboBox;
class QDialogButtonBox;
class QDoubleSpinBox;
class QLineEdit;
class QPlainTextEdit;

class RegistrationDialog : public QDialog {
public:
    explicit RegistrationDialog(QWidget *parent = nullptr);

    void setEvents(const QStringList &eventNames);

    int     getEventIndex()  const;
    QString getFirstName()   const;
    QString getLastName()    const;
    QString getEmail()       const;
    QString getPhone()       const;
    double  getTotalAmount() const;
    QString getNotes()       const;

private:
    QLineEdit       *firstNameEdit;
    QLineEdit       *lastNameEdit;
    QLineEdit       *emailEdit;
    QLineEdit       *phoneEdit;
    QComboBox       *eventEdit;
    QDoubleSpinBox  *totalAmountEdit;
    QPlainTextEdit  *notesEdit;
    QDialogButtonBox *buttonBox;
};

#endif
