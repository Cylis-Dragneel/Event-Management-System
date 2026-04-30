#ifndef VIEWS_REGISTRATIONVIEW_H
#define VIEWS_REGISTRATIONVIEW_H

#include <QWidget>

class QComboBox;
class QDateEdit;
class QLineEdit;
class QPushButton;
class QTableWidget;

class RegistrationView : public QWidget {
public:
    explicit RegistrationView(bool organizerMode, QWidget *parent = nullptr);

private:
    bool isOrganizerMode;
    QLineEdit *searchEdit;
    QComboBox *statusFilter;
    QComboBox *paymentFilter;
    QDateEdit *fromDate;
    QDateEdit *toDate;
    QTableWidget *registrationsTable;
    QPushButton *registerButton;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QPushButton *exportButton;
    QPushButton *refreshButton;
};

#endif
