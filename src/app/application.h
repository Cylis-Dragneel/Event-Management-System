#ifndef APP_APPLICATION_H
#define APP_APPLICATION_H

#include <QApplication>

namespace app {

class Application {
public:
    Application(int &argc, char **argv);
    int run();

private:
    QApplication qtApplication;
};

}  // namespace app

#endif
