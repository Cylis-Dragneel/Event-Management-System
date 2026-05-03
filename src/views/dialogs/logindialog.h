#ifndef VIEWS_DIALOGS_LOGINDIALOG_H
#define VIEWS_DIALOGS_LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;
class Database;
class User;

class LoginDialog : public QDialog {
public:
    explicit LoginDialog(Database *database, QWidget *parent = nullptr);
    User* getUser() const { return currentUser; }

private:
    Database *database;
    User *currentUser;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *signupButton;
    QLabel *errorLabel;

    void attemptLogin();
    void openSignup();
};

#endif