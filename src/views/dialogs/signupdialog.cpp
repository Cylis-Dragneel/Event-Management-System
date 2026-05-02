#include "signupdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "../../models/database.h"
#include "../../models/user.h"

SignupDialog::SignupDialog(Database *db, QWidget *parent)
    : QDialog(parent), database(db), newUser(nullptr) {
    setWindowTitle("Sign Up");
    setFixedSize(400, 350);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);

    auto *titleLabel = new QLabel("Create Account", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1976D2;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(10);

    auto *formLayout = new QVBoxLayout();

    auto *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(new QLabel("Username:", this));
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Choose a username");
    usernameLayout->addWidget(usernameEdit);
    formLayout->addLayout(usernameLayout);

    auto *emailLayout = new QHBoxLayout();
    emailLayout->addWidget(new QLabel("Email:", this));
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("your@email.com");
    emailLayout->addWidget(emailEdit);
    formLayout->addLayout(emailLayout);

    auto *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(new QLabel("Password:", this));
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Create a password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordEdit);
    formLayout->addLayout(passwordLayout);

    auto *confirmLayout = new QHBoxLayout();
    confirmLayout->addWidget(new QLabel("Confirm:", this));
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setPlaceholderText("Re-enter password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmLayout->addWidget(confirmPasswordEdit);
    formLayout->addLayout(confirmLayout);

    auto *roleLayout = new QHBoxLayout();
    roleLayout->addWidget(new QLabel("I am a:", this));
    roleCombo = new QComboBox(this);
    roleCombo->addItem("Attendee", User::ROLE_ATTENDEE);
    roleCombo->addItem("Organizer", User::ROLE_ORGANIZER);
    roleLayout->addWidget(roleCombo);
    formLayout->addLayout(roleLayout);

    mainLayout->addLayout(formLayout);

    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(errorLabel);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *cancelButton = new QPushButton("Cancel", this);
    auto *signupButton = new QPushButton("Sign Up", this);
    signupButton->setDefault(true);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(signupButton);

    mainLayout->addLayout(buttonLayout);

    connect(signupButton, &QPushButton::clicked, this, &SignupDialog::attemptSignup);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void SignupDialog::attemptSignup() {
    QString username = usernameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();
    int role = roleCombo->currentData().toInt();

    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        errorLabel->setText("All fields are required");
        return;
    }

    if (password != confirmPassword) {
        errorLabel->setText("Passwords do not match");
        confirmPasswordEdit->clear();
        return;
    }

    if (password.length() < 4) {
        errorLabel->setText("Password must be at least 4 characters");
        return;
    }

    if (database->userExists(username.toStdString())) {
        errorLabel->setText("Username already exists");
        return;
    }

    User user(username.toStdString(), password.toStdString(), email.toStdString(), role);

    if (database->addUser(user)) {
        newUser = database->loginUser(username.toStdString(), password.toStdString());
        accept();
    } else {
        errorLabel->setText("Failed to create account. Please try again.");
    }
}