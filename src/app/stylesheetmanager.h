#ifndef APP_STYLESHEETMANAGER_H
#define APP_STYLESHEETMANAGER_H

class QApplication;

namespace app {

class StylesheetManager {
public:
    void apply(QApplication &application) const;
};

}  // namespace app

#endif
