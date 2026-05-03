#ifndef VIEWS_EVENTVIEW_H
#define VIEWS_EVENTVIEW_H

#include <QWidget>

#include "../models/event.h"

class QComboBox;
class QDateEdit;
class QLineEdit;
class QPushButton;
class QTableWidget;
class Database;

class EventView : public QWidget {
public:
    explicit EventView(Database *database, bool organizerMode, int currentUserId = -1, QWidget *parent = nullptr);
    ~EventView();

private:
    void ensureEventCapacity(int requiredCount);
    void appendEvent(const Event &event);
    void loadFromDatabase();
    void reloadFromDatabase();
    void rebuildTable();
    void deleteEvent(int index);
    void registerForEvent();

    Database *database;
    int currentUserId;
    bool isOrganizerMode;
    QLineEdit *searchEdit;
    QComboBox *typeFilter;
    QComboBox *statusFilter;
    QDateEdit *fromDate;
    QDateEdit *toDate;
    QTableWidget *eventsTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
    QPushButton *registerButton;

    Event *events;
    int eventCount;
    int eventCapacity;
};

#endif
