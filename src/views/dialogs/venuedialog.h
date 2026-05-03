#ifndef VIEWS_DIALOGS_VENUEDIALOG_H
#define VIEWS_DIALOGS_VENUEDIALOG_H

#include <QDialog>

#include "../../models/venue.h"

class QCheckBox;
class QDialogButtonBox;
class QLineEdit;
class QSpinBox;

class VenueDialog : public QDialog {
public:
    explicit VenueDialog(QWidget *parent = nullptr);

    void populate(const Venue &venue);

    QString getName()          const;
    QString getAddress()       const;
    int     getCapacity()      const;
    QString getContactNumber() const;
    QString getContactEmail()  const;
    bool    getHasWifi()       const;
    bool    getHasParking()    const;
    bool    getHasCatering()   const;
    bool    getHasAVEquipment() const;

private:
    QLineEdit        *nameEdit;
    QLineEdit        *addressEdit;
    QSpinBox         *capacityEdit;
    QLineEdit        *contactNumberEdit;
    QLineEdit        *contactEmailEdit;
    QCheckBox        *wifiCheck;
    QCheckBox        *parkingCheck;
    QCheckBox        *cateringCheck;
    QCheckBox        *avCheck;
    QDialogButtonBox *buttonBox;
};

#endif
