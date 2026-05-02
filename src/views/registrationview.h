#ifndef VIEWS_REGISTRATIONVIEW_H
#define VIEWS_REGISTRATIONVIEW_H

#include <QWidget>

#include "../models/Attendee.h"
#include "../models/Registration.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QTableWidget;
class Database;

class RegistrationView : public QWidget {
public:
    explicit RegistrationView(Database *database, bool organizerMode, QWidget *parent = nullptr);
    ~RegistrationView();

private:
    void loadFromDatabase();
    void rebuildTable();

    Database *database;
    bool isOrganizerMode;
    QLineEdit    *searchEdit;
    QComboBox    *statusFilter;
    QComboBox    *paymentFilter;
    QTableWidget *registrationsTable;
    QPushButton  *registerButton;
    QPushButton  *confirmButton;
    QPushButton  *cancelButton;
    QPushButton  *exportButton;
    QPushButton  *refreshButton;

    Attendee     *attendees;
    Registration *registrations;
    int          regCount;
    int          regCapacity;
    int          nextRegId;

    static const int EVENT_COUNT = 3;
    int  eventIds[EVENT_COUNT];
    char eventNames[EVENT_COUNT][64];
};

#endif
