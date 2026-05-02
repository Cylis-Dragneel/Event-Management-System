#ifndef VIEWS_MAINWINDOW_H
#define VIEWS_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QComboBox;
class QStackedWidget;
class QWidget;

class Database;
class EventView;
class VenueView;
class RegistrationView;
class BudgetView;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(Database *database, QWidget *parent = nullptr);

private:
    void buildUi();
    void applyRole(int roleIndex);

    Database *database;
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

#endif
