#ifndef CCD_SETTING_H
#define CCD_SETTING_H

#include <QtCore/qglobal.h>

#if defined(CCD_LIBRARY)
#  define CCD_API Q_DECL_EXPORT
#else
#  define CCD_API Q_DECL_IMPORT
#endif


#endif // CCD_SETTING_H
