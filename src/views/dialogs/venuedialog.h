#ifndef VIEWS_DIALOGS_VENUEDIALOG_H
#define VIEWS_DIALOGS_VENUEDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QDoubleSpinBox;
class QLineEdit;
class QPlainTextEdit;
class QSpinBox;

namespace views {
namespace dialogs {

class VenueDialog : public QDialog {
public:
    explicit VenueDialog(QWidget *parent = nullptr);

private:
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QSpinBox *capacityEdit;
    QPlainTextEdit *descriptionEdit;
    QLineEdit *amenitiesEdit;
    QDoubleSpinBox *hourlyRateEdit;
    QLineEdit *contactEdit;
    QDialogButtonBox *buttonBox;
};

}  // namespace dialogs
}  // namespace views

#endif
