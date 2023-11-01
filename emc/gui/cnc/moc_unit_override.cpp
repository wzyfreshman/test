/****************************************************************************
** Meta object code from reading C++ file 'unit_override.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_override.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_override.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_unit_override_t {
    QByteArrayData data[12];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_override_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_override_t qt_meta_stringdata_unit_override = {
    {
QT_MOC_LITERAL(0, 0, 13), // "unit_override"
QT_MOC_LITERAL(1, 14, 14), // "sendSetSpindle"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 5), // "slave"
QT_MOC_LITERAL(4, 36, 3), // "vel"
QT_MOC_LITERAL(5, 40, 21), // "set_feed_override_fun"
QT_MOC_LITERAL(6, 62, 24), // "set_spindle_override_fun"
QT_MOC_LITERAL(7, 87, 16), // "set_FeedOverride"
QT_MOC_LITERAL(8, 104, 3), // "val"
QT_MOC_LITERAL(9, 108, 19), // "set_SpindleOverride"
QT_MOC_LITERAL(10, 128, 13), // "set_Feed_slot"
QT_MOC_LITERAL(11, 142, 16) // "set_Spindle_slot"

    },
    "unit_override\0sendSetSpindle\0\0slave\0"
    "vel\0set_feed_override_fun\0"
    "set_spindle_override_fun\0set_FeedOverride\0"
    "val\0set_SpindleOverride\0set_Feed_slot\0"
    "set_Spindle_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_override[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   54,    2, 0x08 /* Private */,
       6,    0,   55,    2, 0x08 /* Private */,
       7,    1,   56,    2, 0x08 /* Private */,
       9,    1,   59,    2, 0x08 /* Private */,
      10,    1,   62,    2, 0x08 /* Private */,
      11,    1,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Double,    8,

       0        // eod
};

void unit_override::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_override *_t = static_cast<unit_override *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSetSpindle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->set_feed_override_fun(); break;
        case 2: _t->set_spindle_override_fun(); break;
        case 3: _t->set_FeedOverride((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->set_SpindleOverride((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->set_Feed_slot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->set_Spindle_slot((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_override::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_override::sendSetSpindle)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject unit_override::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_unit_override.data,
      qt_meta_data_unit_override,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_override::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_override::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_override.stringdata0))
        return static_cast<void*>(const_cast< unit_override*>(this));
    return QWidget::qt_metacast(_clname);
}

int unit_override::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void unit_override::sendSetSpindle(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
