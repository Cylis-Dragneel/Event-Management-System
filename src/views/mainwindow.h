#ifndef VIEWS_MAINWINDOW_H
#define VIEWS_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QStackedWidget;
class QWidget;
class QPushButton;

class Database;
class User;
class EventView;
class VenueView;
class RegistrationView;
class BudgetView;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(Database *database, QWidget *parent = nullptr);

private:
    void buildUi();
    void showLogin();
    void logout();
    void applyUserRole();
    void toggleTheme();

    Database *database;
    User *currentUser;
    QWidget *central;
    QLabel *titleLabel;
    QPushButton *authButton;
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
