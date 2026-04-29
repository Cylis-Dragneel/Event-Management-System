#include "application.h"

#include "stylesheetmanager.h"
#include "../views/mainwindow.h"

namespace app {

Application::Application(int &argc, char **argv) : qtApplication(argc, argv) {
    qtApplication.setApplicationName("Event Management System");
    qtApplication.setOrganizationName("OOP Fast");
}

int Application::run() {
    app::StylesheetManager stylesheetManager;
    stylesheetManager.apply(qtApplication);

    views::MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}

}  // namespace app
