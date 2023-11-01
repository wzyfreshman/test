/****************************************************************************
** Meta object code from reading C++ file 'other.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/other.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'other.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Other_t {
    QByteArrayData data[12];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Other_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Other_t qt_meta_stringdata_Other = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Other"
QT_MOC_LITERAL(1, 6, 7), // "sendMsg"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(4, 37, 13), // "set_debug_num"
QT_MOC_LITERAL(5, 51, 10), // "set_io_num"
QT_MOC_LITERAL(6, 62, 23), // "on_CB_Version_activated"
QT_MOC_LITERAL(7, 86, 5), // "index"
QT_MOC_LITERAL(8, 92, 25), // "on_CB_converter_activated"
QT_MOC_LITERAL(9, 118, 23), // "on_CB_ioOrder_activated"
QT_MOC_LITERAL(10, 142, 13), // "set_max_speed"
QT_MOC_LITERAL(11, 156, 21) // "on_CB_Servo_activated"

    },
    "Other\0sendMsg\0\0on_pushButton_clicked\0"
    "set_debug_num\0set_io_num\0"
    "on_CB_Version_activated\0index\0"
    "on_CB_converter_activated\0"
    "on_CB_ioOrder_activated\0set_max_speed\0"
    "on_CB_Servo_activated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Other[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   64,    2, 0x08 /* Private */,
       4,    0,   65,    2, 0x08 /* Private */,
       5,    0,   66,    2, 0x08 /* Private */,
       6,    1,   67,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x08 /* Private */,
       9,    1,   73,    2, 0x08 /* Private */,
      10,    0,   76,    2, 0x08 /* Private */,
      11,    1,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void Other::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Other *_t = static_cast<Other *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->on_pushButton_clicked(); break;
        case 2: _t->set_debug_num(); break;
        case 3: _t->set_io_num(); break;
        case 4: _t->on_CB_Version_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_CB_converter_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_CB_ioOrder_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->set_max_speed(); break;
        case 8: _t->on_CB_Servo_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Other::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Other::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Other::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Other.data,
      qt_meta_data_Other,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Other::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Other::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Other.stringdata0))
        return static_cast<void*>(const_cast< Other*>(this));
    return QDialog::qt_metacast(_clname);
}

int Other::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Other::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
