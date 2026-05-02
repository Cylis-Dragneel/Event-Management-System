#include "mainwindow.h"

#include "budgetview.h"
#include "eventview.h"
#include "registrationview.h"
#include "venueview.h"
#include "dialogs/logindialog.h"

#include <QApplication>
#include <QComboBox>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "../models/user.h"

MainWindow::MainWindow(Database *db, QWidget *parent)
    : QMainWindow(parent),
      database(db),
      currentUser(nullptr),
      central(nullptr),
      titleLabel(nullptr),
      authButton(nullptr),
      themeButton(nullptr),
      roleStack(nullptr),
      isDarkMode(false),
      organizerEventView(nullptr),
      organizerVenueView(nullptr),
      organizerRegistrationView(nullptr),
      organizerBudgetView(nullptr),
      attendeeEventView(nullptr),
      attendeeRegistrationView(nullptr) {
    buildUi();
    showLogin();
}

void MainWindow::buildUi() {
    setWindowTitle("Event Management System");
    resize(1280, 820);

    central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    auto *headerLayout = new QHBoxLayout();
    titleLabel = new QLabel("Event Management Dashboard", central);
    titleLabel->setObjectName("titleLabel");

    themeButton = new QPushButton(central);
    themeButton->setObjectName("themeToggle");
    themeButton->setText("Dark Mode");
    themeButton->setFixedWidth(100);
    themeButton->setToolTip("Toggle Dark/Light Mode");

    authButton = new QPushButton("Login", central);
    authButton->setFixedWidth(80);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(themeButton);
    headerLayout->addWidget(authButton);
    mainLayout->addLayout(headerLayout);

    roleStack = new QStackedWidget(central);

    auto *organizerTabs = new QTabWidget(roleStack);
    organizerEventView = new EventView(database, true, organizerTabs);
    organizerVenueView = new VenueView(database, true, organizerTabs);
    organizerRegistrationView = new RegistrationView(database, true, organizerTabs);
    organizerBudgetView = new BudgetView(database, true, organizerTabs);
    organizerTabs->addTab(organizerEventView, "Events");
    organizerTabs->addTab(organizerVenueView, "Venues");
    organizerTabs->addTab(organizerRegistrationView, "Registrations");
    organizerTabs->addTab(organizerBudgetView, "Budgets");

    auto *attendeeTabs = new QTabWidget(roleStack);
    attendeeEventView = new EventView(database, false, attendeeTabs);
    attendeeRegistrationView = new RegistrationView(database, false, attendeeTabs);
    attendeeTabs->addTab(attendeeEventView, "Browse Events");
    attendeeTabs->addTab(attendeeRegistrationView, "My Registrations");

    roleStack->addWidget(organizerTabs);
    roleStack->addWidget(attendeeTabs);
    mainLayout->addWidget(roleStack);

    setCentralWidget(central);

    QObject::connect(authButton, &QPushButton::clicked, this, [this]() {
        if (currentUser) {
            logout();
        } else {
            showLogin();
        }
    });

    QObject::connect(themeButton, &QPushButton::clicked, this, [this]() {
        toggleTheme();
    });
}

void MainWindow::showLogin() {
    LoginDialog loginDialog(database, this);
    if (loginDialog.exec() == QDialog::Accepted && loginDialog.getUser()) {
        currentUser = loginDialog.getUser();
        applyUserRole();
    } else {
        if (!currentUser) {
            qApp->quit();
        }
    }
}

void MainWindow::applyUserRole() {
    if (currentUser->isOrganizer()) {
        titleLabel->setText("Event Management Dashboard");
        roleStack->setCurrentIndex(0);
        authButton->setText("Logout");
    } else {
        titleLabel->setText("Event Discovery");
        roleStack->setCurrentIndex(1);
        authButton->setText("Logout");
    }
}

void MainWindow::logout() {
    if (currentUser) {
        delete currentUser;
        currentUser = nullptr;
    }
    titleLabel->setText("Event Management Dashboard");
    authButton->setText("Login");
    roleStack->setCurrentIndex(0);
    showLogin();
}

void MainWindow::toggleTheme() {
    isDarkMode = !isDarkMode;

    QString stylesheet;
    if (isDarkMode) {
        QFile file(":/styles/dark.qss");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stylesheet = stream.readAll();
        }
        themeButton->setText("Light Mode");
    } else {
        QFile file(":/styles/material.qss");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stylesheet = stream.readAll();
        }
        themeButton->setText("Dark Mode");
    }

    qApp->setStyleSheet(stylesheet);
}