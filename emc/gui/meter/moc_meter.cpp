/****************************************************************************
** Meta object code from reading C++ file 'meter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "meter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Meter_t {
    QByteArrayData data[7];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Meter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Meter_t qt_meta_stringdata_Meter = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Meter"
QT_MOC_LITERAL(1, 6, 12), // "updateStatus"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 12), // "getPinChange"
QT_MOC_LITERAL(4, 33, 12), // "getSigChange"
QT_MOC_LITERAL(5, 46, 12), // "getParChange"
QT_MOC_LITERAL(6, 59, 10) // "showUpdate"

    },
    "Meter\0updateStatus\0\0getPinChange\0"
    "getSigChange\0getParChange\0showUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Meter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    2,   40,    2, 0x0a /* Public */,
       4,    2,   45,    2, 0x0a /* Public */,
       5,    2,   50,    2, 0x0a /* Public */,
       6,    0,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,

       0        // eod
};

void Meter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Meter *_t = static_cast<Meter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateStatus(); break;
        case 1: _t->getPinChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->getSigChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->getParChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->showUpdate(); break;
        default: ;
        }
    }
}

const QMetaObject Meter::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Meter.data,
      qt_meta_data_Meter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Meter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Meter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Meter.stringdata0))
        return static_cast<void*>(const_cast< Meter*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Meter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
