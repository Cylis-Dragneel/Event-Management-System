#ifndef VIEWS_MAINWINDOW_H
#define VIEWS_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QComboBox;
class QStackedWidget;
class QWidget;

namespace views {

class EventView;
class VenueView;
class RegistrationView;
class BudgetView;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();
    void applyRole(int roleIndex);

    QWidget *central;
    QLabel *titleLabel;
    QComboBox *roleSelector;
    QStackedWidget *roleStack;

    EventView *organizerEventView;
    VenueView *organizerVenueView;
    RegistrationView *organizerRegistrationView;
    BudgetView *organizerBudgetView;

    EventView *attendeeEventView;
    RegistrationView *attendeeRegistrationView;
};

}

#endif
