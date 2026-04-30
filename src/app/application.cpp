#include "application.h"

#include "stylesheetmanager.h"
#include "../views/mainwindow.h"

Application::Application(int &argc, char **argv) : qtApplication(argc, argv) {
    qtApplication.setApplicationName("Event Management System");
    qtApplication.setOrganizationName("OOP Fast");
}

int Application::run() {
    StylesheetManager stylesheetManager;
    stylesheetManager.apply(qtApplication);

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
