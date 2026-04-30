#ifndef VIEWS_DIALOGS_EVENTDIALOG_H
#define VIEWS_DIALOGS_EVENTDIALOG_H

#include <QDialog>

class QComboBox;
class QDateTimeEdit;
class QDialogButtonBox;
class QPlainTextEdit;
class QSpinBox;
class QLineEdit;

class EventDialog : public QDialog {
public:
    explicit EventDialog(QWidget *parent = nullptr);

private:
    QLineEdit *nameEdit;
    QPlainTextEdit *descriptionEdit;
    QDateTimeEdit *dateTimeEdit;
    QSpinBox *durationEdit;
    QComboBox *typeEdit;
    QSpinBox *capacityEdit;
    QLineEdit *venueEdit;
    QComboBox *statusEdit;
    QDialogButtonBox *buttonBox;
};

#endif
