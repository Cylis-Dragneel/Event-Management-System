#include "app/application.h"

int main(int argc, char *argv[]) {
    app::Application application(argc, argv);
    return application.run();
}
