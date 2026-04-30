#include "mainwindow.h"

#include "budgetview.h"
#include "eventview.h"
#include "registrationview.h"
#include "venueview.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      central(nullptr),
      titleLabel(nullptr),
      roleSelector(nullptr),
      roleStack(nullptr),
      organizerEventView(nullptr),
      organizerVenueView(nullptr),
      organizerRegistrationView(nullptr),
      organizerBudgetView(nullptr),
      attendeeEventView(nullptr),
      attendeeRegistrationView(nullptr) {
    buildUi();
    applyRole(0);
}

void MainWindow::buildUi() {
    setWindowTitle("Event Management System");
    resize(1280, 820);

    central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    auto *headerLayout = new QHBoxLayout();
    titleLabel = new QLabel("Event Management Dashboard", central);
    titleLabel->setObjectName("titleLabel");
    roleSelector = new QComboBox(central);
    roleSelector->addItems({"Organizer", "Attendee"});
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(new QLabel("View", central));
    headerLayout->addWidget(roleSelector);
    mainLayout->addLayout(headerLayout);

    roleStack = new QStackedWidget(central);

    auto *organizerTabs = new QTabWidget(roleStack);
    organizerEventView = new EventView(true, organizerTabs);
    organizerVenueView = new VenueView(true, organizerTabs);
    organizerRegistrationView = new RegistrationView(true, organizerTabs);
    organizerBudgetView = new BudgetView(true, organizerTabs);
    organizerTabs->addTab(organizerEventView, "Events");
    organizerTabs->addTab(organizerVenueView, "Venues");
    organizerTabs->addTab(organizerRegistrationView, "Registrations");
    organizerTabs->addTab(organizerBudgetView, "Budgets");

    auto *attendeeTabs = new QTabWidget(roleStack);
    attendeeEventView = new EventView(false, attendeeTabs);
    attendeeRegistrationView = new RegistrationView(false, attendeeTabs);
    attendeeTabs->addTab(attendeeEventView, "Browse Events");
    attendeeTabs->addTab(attendeeRegistrationView, "My Registrations");

    roleStack->addWidget(organizerTabs);
    roleStack->addWidget(attendeeTabs);
    mainLayout->addWidget(roleStack);

    setCentralWidget(central);

    QObject::connect(roleSelector, &QComboBox::currentIndexChanged, this, [this](int index) {
        applyRole(index);
    });
}

void MainWindow::applyRole(int roleIndex) {
    roleStack->setCurrentIndex(roleIndex);
    if (roleIndex == 0) {
        titleLabel->setText("Event Management Dashboard");
    } else {
        titleLabel->setText("Event Discovery");
    }
}

