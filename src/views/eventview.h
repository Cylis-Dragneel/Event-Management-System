#ifndef VIEWS_EVENTVIEW_H
#define VIEWS_EVENTVIEW_H

#include <QWidget>

#include "../models/event.h"

class QComboBox;
class QDateEdit;
class QLineEdit;
class QPushButton;
class QTableWidget;

class EventView : public QWidget {
public:
    explicit EventView(bool organizerMode, QWidget *parent = nullptr);
    ~EventView();

private:
    void ensureEventCapacity(int requiredCount);
    void appendEvent(const Event &event);
    void seedEvents();
    void rebuildTable();
    void deleteEvent(int index);

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

    Event *events;
    int eventCount;
    int eventCapacity;
};

#endif
