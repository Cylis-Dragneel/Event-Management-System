#include "logindialog.h"
#include "signupdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "../../models/database.h"
#include "../../models/user.h"

LoginDialog::LoginDialog(Database *db, QWidget *parent)
    : QDialog(parent), database(db), currentUser(nullptr) {
    setWindowTitle("Login");
    setFixedSize(400, 250);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);

    auto *titleLabel = new QLabel("Event Management System", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1976D2;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);

    auto *formLayout = new QVBoxLayout();

    auto *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(new QLabel("Username:", this));
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Enter username");
    usernameLayout->addWidget(usernameEdit);
    formLayout->addLayout(usernameLayout);

    auto *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(new QLabel("Password:", this));
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Enter password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordEdit);
    formLayout->addLayout(passwordLayout);

    mainLayout->addLayout(formLayout);

    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(errorLabel);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    signupButton = new QPushButton("Sign Up", this);
    loginButton = new QPushButton("Login", this);
    loginButton->setDefault(true);

    buttonLayout->addWidget(signupButton);
    buttonLayout->addWidget(loginButton);

    mainLayout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    connect(signupButton, &QPushButton::clicked, this, &LoginDialog::openSignup);

    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginDialog::attemptLogin);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::attemptLogin);
}

void LoginDialog::attemptLogin() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Please enter username and password");
        return;
    }

    User *user = database->loginUser(username.toStdString(), password.toStdString());

    if (user) {
        currentUser = user;
        accept();
    } else {
        errorLabel->setText("Invalid username or password");
        passwordEdit->clear();
    }
}

void LoginDialog::openSignup() {
    SignupDialog signup(database, this);
    if (signup.exec() == QDialog::Accepted) {
        User *newUser = signup.getUser();
        if (newUser) {
            currentUser = newUser;
            accept();
        }
    }
}