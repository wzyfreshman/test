#ifndef CD_INISETTINGS_H
#define CD_INISETTINGS_H

#include <QSettings>
#include "ccd_setting.h"

class CCD_API CD_IniSettings:public QSettings {

public:
    CD_IniSettings(const QString name);

};

#endif // CD_INISETTINGS_H
