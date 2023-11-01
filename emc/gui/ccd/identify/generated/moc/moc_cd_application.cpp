/****************************************************************************
** Meta object code from reading C++ file 'cd_application.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cd_application.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cd_application.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CD_Application[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      47,   42,   15,   15, 0x0a,
      90,   77,   15,   15, 0x0a,
     124,  118,   15,   15, 0x0a,
     152,   15,   15,   15, 0x2a,
     177,  118,   15,   15, 0x0a,
     203,   15,   15,   15, 0x2a,
     233,  226,   15,   15, 0x0a,
     278,   77,  268,   15, 0x0a,
     302,   15,   15,   15, 0x0a,
     320,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CD_Application[] = {
    "CD_Application\0\0actionUpdateDisplayTemp()\0"
    "mark\0actionEditTemplate(CCD_Mark*)\0"
    "camera,index\0actionEditTemplate(int,int)\0"
    "index\0actionEditSysParameter(int)\0"
    "actionEditSysParameter()\0"
    "actionEditeCommonPar(int)\0"
    "actionEditeCommonPar()\0mat,To\0"
    "doImgCompensate(cv::Mat&,cv::Mat&)\0"
    "RS_Vector\0actionIdentify(int,int)\0"
    "slotClearScreen()\0back()\0"
};

void CD_Application::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CD_Application *_t = static_cast<CD_Application *>(_o);
        switch (_id) {
        case 0: _t->actionUpdateDisplayTemp(); break;
        case 1: _t->actionEditTemplate((*reinterpret_cast< CCD_Mark*(*)>(_a[1]))); break;
        case 2: _t->actionEditTemplate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->actionEditSysParameter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->actionEditSysParameter(); break;
        case 5: _t->actionEditeCommonPar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->actionEditeCommonPar(); break;
        case 7: _t->doImgCompensate((*reinterpret_cast< cv::Mat(*)>(_a[1])),(*reinterpret_cast< cv::Mat(*)>(_a[2]))); break;
        case 8: { RS_Vector _r = _t->actionIdentify((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< RS_Vector*>(_a[0]) = _r; }  break;
        case 9: _t->slotClearScreen(); break;
        case 10: _t->back(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CD_Application::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CD_Application::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CD_Application,
      qt_meta_data_CD_Application, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CD_Application::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CD_Application::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CD_Application::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CD_Application))
        return static_cast<void*>(const_cast< CD_Application*>(this));
    if (!strcmp(_clname, "CCD_MarksFace"))
        return static_cast< CCD_MarksFace*>(const_cast< CD_Application*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< CD_Application*>(this));
    return QObject::qt_metacast(_clname);
}

int CD_Application::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
