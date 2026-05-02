#include "application.h"

#include "stylesheetmanager.h"
#include "../views/mainwindow.h"
#include "../models/database.h"

Application::Application(int &argc, char **argv) : qtApplication(argc, argv), database(nullptr) {
    qtApplication.setApplicationName("Event Management System");
    qtApplication.setOrganizationName("OOP Fast");

    database = new Database();
    if (!database->connectDatabase()) {
        qDebug() << "Failed to connect to database!";
    } else {
        database->createTables();
        qDebug() << "Database initialized successfully";
    }
}

Application::~Application() {
    if (database) {
        database->closeDatabase();
        delete database;
    }
}

int Application::run() {
    StylesheetManager stylesheetManager;
    stylesheetManager.apply(qtApplication);

    MainWindow mainWindow(database);
    mainWindow.show();

    return QApplication::exec();
}
