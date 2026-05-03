#ifndef VIEWS_REGISTRATIONVIEW_H
#define VIEWS_REGISTRATIONVIEW_H

#include <QWidget>

#include "../models/Attendee.h"
#include "../models/Registration.h"

#include <QMap>
#include <QString>

class QComboBox;
class QLineEdit;
class QPushButton;
class QTableWidget;
class Database;
class User;

class RegistrationView : public QWidget {
public:
    explicit RegistrationView(Database *database, bool organizerMode, User *currentUser, QWidget *parent = nullptr);
    ~RegistrationView();

private:
    void loadFromDatabase();
    void reloadFromDatabase();
    void rebuildTable();
    void unregister();
    void makePayment();

    Database *database;
    User *currentUser;
    bool isOrganizerMode;
    QLineEdit    *searchEdit;
    QComboBox    *statusFilter;
    QComboBox    *paymentFilter;
    QTableWidget *registrationsTable;
    QPushButton  *registerButton;
    QPushButton  *confirmButton;
    QPushButton  *cancelButton;
    QPushButton  *payButton;
    QPushButton  *exportButton;
    QPushButton  *refreshButton;

    Attendee     *attendees;
    Registration *registrations;
    int          regCount;
    int          regCapacity;
    int          nextRegId;

    QMap<int, QString> eventNames;
};

#endif
