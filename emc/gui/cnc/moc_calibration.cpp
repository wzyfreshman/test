/****************************************************************************
** Meta object code from reading C++ file 'calibration.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/calibration.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Calibration_t {
    QByteArrayData data[21];
    char stringdata0[315];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Calibration_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Calibration_t qt_meta_stringdata_Calibration = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Calibration"
QT_MOC_LITERAL(1, 12, 7), // "sendMsg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 10), // "updatecoor"
QT_MOC_LITERAL(4, 32, 18), // "changeValueOfDpeth"
QT_MOC_LITERAL(5, 51, 5), // "index"
QT_MOC_LITERAL(6, 57, 9), // "addwidget"
QT_MOC_LITERAL(7, 67, 10), // "addconnect"
QT_MOC_LITERAL(8, 78, 8), // "show_abs"
QT_MOC_LITERAL(9, 87, 14), // "set_p1y_offset"
QT_MOC_LITERAL(10, 102, 16), // "set_p1ver_offset"
QT_MOC_LITERAL(11, 119, 14), // "set_p2y_offset"
QT_MOC_LITERAL(12, 134, 16), // "set_p2ver_offset"
QT_MOC_LITERAL(13, 151, 20), // "set_location1_offset"
QT_MOC_LITERAL(14, 172, 20), // "set_location2_offset"
QT_MOC_LITERAL(15, 193, 17), // "set_holder_offset"
QT_MOC_LITERAL(16, 211, 20), // "set_location3_offset"
QT_MOC_LITERAL(17, 232, 20), // "set_location4_offset"
QT_MOC_LITERAL(18, 253, 18), // "set_mholder_offset"
QT_MOC_LITERAL(19, 272, 20), // "set_leftdepthtoplank"
QT_MOC_LITERAL(20, 293, 21) // "set_rightdepthtoplank"

    },
    "Calibration\0sendMsg\0\0updatecoor\0"
    "changeValueOfDpeth\0index\0addwidget\0"
    "addconnect\0show_abs\0set_p1y_offset\0"
    "set_p1ver_offset\0set_p2y_offset\0"
    "set_p2ver_offset\0set_location1_offset\0"
    "set_location2_offset\0set_holder_offset\0"
    "set_location3_offset\0set_location4_offset\0"
    "set_mholder_offset\0set_leftdepthtoplank\0"
    "set_rightdepthtoplank"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Calibration[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  109,    2, 0x0a /* Public */,
       4,    1,  110,    2, 0x0a /* Public */,
       6,    0,  113,    2, 0x08 /* Private */,
       7,    0,  114,    2, 0x08 /* Private */,
       8,    0,  115,    2, 0x08 /* Private */,
       9,    0,  116,    2, 0x08 /* Private */,
      10,    0,  117,    2, 0x08 /* Private */,
      11,    0,  118,    2, 0x08 /* Private */,
      12,    0,  119,    2, 0x08 /* Private */,
      13,    0,  120,    2, 0x08 /* Private */,
      14,    0,  121,    2, 0x08 /* Private */,
      15,    0,  122,    2, 0x08 /* Private */,
      16,    0,  123,    2, 0x08 /* Private */,
      17,    0,  124,    2, 0x08 /* Private */,
      18,    0,  125,    2, 0x08 /* Private */,
      19,    0,  126,    2, 0x08 /* Private */,
      20,    0,  127,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Calibration::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Calibration *_t = static_cast<Calibration *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->updatecoor(); break;
        case 2: _t->changeValueOfDpeth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->addwidget(); break;
        case 4: _t->addconnect(); break;
        case 5: _t->show_abs(); break;
        case 6: _t->set_p1y_offset(); break;
        case 7: _t->set_p1ver_offset(); break;
        case 8: _t->set_p2y_offset(); break;
        case 9: _t->set_p2ver_offset(); break;
        case 10: _t->set_location1_offset(); break;
        case 11: _t->set_location2_offset(); break;
        case 12: _t->set_holder_offset(); break;
        case 13: _t->set_location3_offset(); break;
        case 14: _t->set_location4_offset(); break;
        case 15: _t->set_mholder_offset(); break;
        case 16: _t->set_leftdepthtoplank(); break;
        case 17: _t->set_rightdepthtoplank(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Calibration::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Calibration::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Calibration::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Calibration.data,
      qt_meta_data_Calibration,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Calibration::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Calibration::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Calibration.stringdata0))
        return static_cast<void*>(const_cast< Calibration*>(this));
    return QDialog::qt_metacast(_clname);
}

int Calibration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void Calibration::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
