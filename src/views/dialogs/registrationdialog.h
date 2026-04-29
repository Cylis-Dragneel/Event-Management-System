#ifndef VIEWS_DIALOGS_REGISTRATIONDIALOG_H
#define VIEWS_DIALOGS_REGISTRATIONDIALOG_H

#include <QDialog>

class QComboBox;
class QDateTimeEdit;
class QDialogButtonBox;
class QLineEdit;
class QPlainTextEdit;

namespace views {
namespace dialogs {

class RegistrationDialog : public QDialog {
public:
    explicit RegistrationDialog(QWidget *parent = nullptr);

private:
    QLineEdit *attendeeNameEdit;
    QLineEdit *attendeeEmailEdit;
    QLineEdit *eventEdit;
    QDateTimeEdit *registrationDateEdit;
    QComboBox *statusEdit;
    QComboBox *paymentStatusEdit;
    QPlainTextEdit *notesEdit;
    QDialogButtonBox *buttonBox;
};

}  // namespace dialogs
}  // namespace views

#endif
