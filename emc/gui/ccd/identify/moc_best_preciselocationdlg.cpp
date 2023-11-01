/****************************************************************************
** Meta object code from reading C++ file 'best_preciselocationdlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "best/best_preciselocationdlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'best_preciselocationdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Best_PreciseLocationDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x08,
      46,   24,   24,   24, 0x08,
      66,   24,   24,   24, 0x08,
      88,   24,   24,   24, 0x08,
     112,   24,   24,   24, 0x08,
     139,   24,   24,   24, 0x08,
     165,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Best_PreciseLocationDlg[] = {
    "Best_PreciseLocationDlg\0\0on_Btn_CCD_clicked()\0"
    "on_Btn_Go_clicked()\0on_Btn_Sure_clicked()\0"
    "on_Btn_Cancel_clicked()\0"
    "on_SetCoordinate_clicked()\0"
    "on_Btn_DoOffset_clicked()\0"
    "on_Btn_OpenHandle_clicked()\0"
};

void Best_PreciseLocationDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Best_PreciseLocationDlg *_t = static_cast<Best_PreciseLocationDlg *>(_o);
        switch (_id) {
        case 0: _t->on_Btn_CCD_clicked(); break;
        case 1: _t->on_Btn_Go_clicked(); break;
        case 2: _t->on_Btn_Sure_clicked(); break;
        case 3: _t->on_Btn_Cancel_clicked(); break;
        case 4: _t->on_SetCoordinate_clicked(); break;
        case 5: _t->on_Btn_DoOffset_clicked(); break;
        case 6: _t->on_Btn_OpenHandle_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Best_PreciseLocationDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Best_PreciseLocationDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Best_PreciseLocationDlg,
      qt_meta_data_Best_PreciseLocationDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Best_PreciseLocationDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Best_PreciseLocationDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Best_PreciseLocationDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Best_PreciseLocationDlg))
        return static_cast<void*>(const_cast< Best_PreciseLocationDlg*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< Best_PreciseLocationDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int Best_PreciseLocationDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
