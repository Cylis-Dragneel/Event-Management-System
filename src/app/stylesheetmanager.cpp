#include "stylesheetmanager.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

namespace app {

void StylesheetManager::apply(QApplication &application) const {
    QFile stylesheetFile(":/styles/material.qss");
    if (stylesheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&stylesheetFile);
        application.setStyleSheet(stream.readAll());
    }
}

}  // namespace app
