#ifndef VIEWS_VENUEVIEW_H
#define VIEWS_VENUEVIEW_H

#include <QWidget>

class QComboBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTableWidget;

namespace views {

class VenueView : public QWidget {
public:
    explicit VenueView(bool organizerMode, QWidget *parent = nullptr);

private:
    bool isOrganizerMode;
    QLineEdit *searchEdit;
    QSpinBox *capacityFilter;
    QComboBox *amenityFilter;
    QTableWidget *venuesTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
};

}  // namespace views

#endif
