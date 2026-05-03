#ifndef VIEWS_VENUEVIEW_H
#define VIEWS_VENUEVIEW_H

#include <QWidget>

#include "../models/venue.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTableWidget;
class Database;

class VenueView : public QWidget {
public:
    explicit VenueView(Database *database, bool organizerMode, QWidget *parent = nullptr);
    ~VenueView();

private:
    void ensureVenueCapacity(int requiredCount);
    void appendVenue(const Venue &venue);
    void loadFromDatabase();
    void rebuildTable();
    void deleteVenue(int index);

    Database *database;
    bool isOrganizerMode;
    QLineEdit *searchEdit;
    QSpinBox *capacityFilter;
    QComboBox *amenityFilter;
    QTableWidget *venuesTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;

    Venue *venues;
    int venueCount;
    int venueCapacity;
};

#endif
