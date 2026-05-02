#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QString>
#include <string>
#include "Attendee.h"
#include "venue.h"
#include "event.h"
#include "Registration.h"
#include "BudgetItem.h"


class Database {
private:
    QSqlDatabase db;

public:
    Database();
    ~Database();

    bool connectDatabase();
    void closeDatabase();
    bool createTables();

    // ================= ATTENDEE OPERATIONS =================
    bool addAttendee(const Attendee& attendee);
    Attendee getAttendee(int id);
    // Venue Operations
    bool addVenue(const Venue& venue);
    Venue getVenue(int id);

    // Event Operations
    bool addEvent(const Event& event);
    Event getEvent(int id);
    // Registration Operations
    bool addRegistration(const Registration& reg);
    Registration getRegistration(int id);

    // BudgetItem Operations
    bool addBudgetItem(const BudgetItem& item);
    BudgetItem getBudgetItem(int id);

    // ================= UPDATE & DELETE OPERATIONS =================
    bool updateAttendee(const Attendee& attendee);
    bool deleteAttendee(int id);

    bool updateVenue(int id, const Venue& venue);
    bool deleteVenue(int id);

    bool updateEvent(int id, const Event& event);
    bool deleteEvent(int id);

    bool updateRegistration(int id, const Registration& reg);
    bool deleteRegistration(int id);

    bool updateBudgetItem(int id, const BudgetItem& item);
    bool deleteBudgetItem(int id);

};

#endif // DATABASE_H
