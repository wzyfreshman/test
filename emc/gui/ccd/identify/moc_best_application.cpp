/****************************************************************************
** Meta object code from reading C++ file 'best_application.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "best/best_application.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'best_application.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Best_Application[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      48,   17,   17,   17, 0x08,
      76,   17,   17,   17, 0x08,
     102,   17,   17,   17, 0x08,
     127,   17,   17,   17, 0x08,
     152,   17,   17,   17, 0x08,
     180,   17,   17,   17, 0x08,
     207,   17,   17,   17, 0x08,
     232,   17,   17,   17, 0x08,
     260,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Best_Application[] = {
    "Best_Application\0\0on_Btn_PositionEdit_clicked()\0"
    "on_Btn_ProductPar_clicked()\0"
    "on_Btn_Electric_clicked()\0"
    "on_Btn_TestFun_clicked()\0"
    "on_Btn_SendCmd_clicked()\0"
    "on_Btn_TestSelect_clicked()\0"
    "on_Btn_ClearWait_clicked()\0"
    "on_Btn_TestRef_clicked()\0"
    "on_Btn_ReadResult_clicked()\0"
    "on_Btn_SavePar_clicked()\0"
};

void Best_Application::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Best_Application *_t = static_cast<Best_Application *>(_o);
        switch (_id) {
        case 0: _t->on_Btn_PositionEdit_clicked(); break;
        case 1: _t->on_Btn_ProductPar_clicked(); break;
        case 2: _t->on_Btn_Electric_clicked(); break;
        case 3: _t->on_Btn_TestFun_clicked(); break;
        case 4: _t->on_Btn_SendCmd_clicked(); break;
        case 5: _t->on_Btn_TestSelect_clicked(); break;
        case 6: _t->on_Btn_ClearWait_clicked(); break;
        case 7: _t->on_Btn_TestRef_clicked(); break;
        case 8: _t->on_Btn_ReadResult_clicked(); break;
        case 9: _t->on_Btn_SavePar_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Best_Application::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Best_Application::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Best_Application,
      qt_meta_data_Best_Application, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Best_Application::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Best_Application::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Best_Application::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Best_Application))
        return static_cast<void*>(const_cast< Best_Application*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< Best_Application*>(this));
    return QWidget::qt_metacast(_clname);
}

int Best_Application::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
