#ifndef VIEWS_EVENTVIEW_H
#define VIEWS_EVENTVIEW_H

#include <QWidget>

class QComboBox;
class QDateEdit;
class QLineEdit;
class QPushButton;
class QTableWidget;

namespace views {

class EventView : public QWidget {
public:
    explicit EventView(bool organizerMode, QWidget *parent = nullptr);

private:
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
};

}  // namespace views

#endif
