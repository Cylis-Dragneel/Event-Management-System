#ifndef APP_APPLICATION_H
#define APP_APPLICATION_H

#include <QApplication>

class Database;

class Application {
public:
    Application(int &argc, char **argv);
    ~Application();
    int run();
    Database* getDatabase() { return database; }

private:
    QApplication qtApplication;
    Database *database;
};

#endif
