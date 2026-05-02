#ifndef VIEWS_DIALOGS_SIGNUPDIALOG_H
#define VIEWS_DIALOGS_SIGNUPDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;
class Database;
class User;

class SignupDialog : public QDialog {
public:
    explicit SignupDialog(Database *database, QWidget *parent = nullptr);
    User* getUser() const { return newUser; }

private:
    Database *database;
    User *newUser;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QLineEdit *emailEdit;
    QComboBox *roleCombo;
    QLabel *errorLabel;

    void attemptSignup();
};

#endif