#ifndef VIEWS_DIALOGS_EVENTDIALOG_H
#define VIEWS_DIALOGS_EVENTDIALOG_H

#include <QDialog>

#include "../../models/event.h"

class QComboBox;
class QDateTimeEdit;
class QDialogButtonBox;
class QPlainTextEdit;
class QSpinBox;
class QLineEdit;

class EventDialog : public QDialog {
public:
    explicit EventDialog(QWidget *parent = nullptr);

    void populate(const Event &event);

    QString getName()        const;
    QString getDescription() const;
    QString getDate()        const;
    QString getTime()        const;
    int     getDuration()    const;
    int     getType()        const;
    int     getCapacity()    const;
    int     getVenueId()     const;
    int     getStatus()      const;

private:
    QLineEdit        *nameEdit;
    QPlainTextEdit   *descriptionEdit;
    QDateTimeEdit    *dateTimeEdit;
    QSpinBox         *durationEdit;
    QComboBox        *typeEdit;
    QSpinBox         *capacityEdit;
    QSpinBox         *venueEdit;
    QComboBox        *statusEdit;
    QDialogButtonBox *buttonBox;
};

#endif
