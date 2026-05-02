#ifndef VIEWS_MAINWINDOW_H
#define VIEWS_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QComboBox;
class QStackedWidget;
class QWidget;
class QPushButton;

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
    void toggleTheme();

    Database *database;
    QWidget *central;
    QLabel *titleLabel;
    QComboBox *roleSelector;
    QPushButton *themeButton;
    QStackedWidget *roleStack;
    bool isDarkMode;

    EventView *organizerEventView;
    VenueView *organizerVenueView;
    RegistrationView *organizerRegistrationView;
    BudgetView *organizerBudgetView;

    EventView *attendeeEventView;
    RegistrationView *attendeeRegistrationView;
};

#endif
