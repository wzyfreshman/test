/****************************************************************************
** Meta object code from reading C++ file 'unit_tooltimes.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_tooltimes.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_tooltimes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_tooltimes_t {
    QByteArrayData data[14];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tooltimes_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tooltimes_t qt_meta_stringdata_tooltimes = {
    {
QT_MOC_LITERAL(0, 0, 9), // "tooltimes"
QT_MOC_LITERAL(1, 10, 8), // "send_msg"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 3), // "str"
QT_MOC_LITERAL(4, 24, 8), // "setvalue"
QT_MOC_LITERAL(5, 33, 1), // "x"
QT_MOC_LITERAL(6, 35, 1), // "y"
QT_MOC_LITERAL(7, 37, 9), // "setheader"
QT_MOC_LITERAL(8, 47, 5), // "index"
QT_MOC_LITERAL(9, 53, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(10, 75, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(11, 99, 6), // "update"
QT_MOC_LITERAL(12, 106, 17), // "on_usetbl_clicked"
QT_MOC_LITERAL(13, 124, 7) // "checked"

    },
    "tooltimes\0send_msg\0\0str\0setvalue\0x\0y\0"
    "setheader\0index\0on_pushButton_clicked\0"
    "on_pushButton_2_clicked\0update\0"
    "on_usetbl_clicked\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tooltimes[] = {

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
       4,    2,   54,    2, 0x0a /* Public */,
       7,    1,   59,    2, 0x0a /* Public */,
       9,    0,   62,    2, 0x0a /* Public */,
      10,    0,   63,    2, 0x08 /* Private */,
      11,    0,   64,    2, 0x08 /* Private */,
      12,    1,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,

       0        // eod
};

void tooltimes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        tooltimes *_t = static_cast<tooltimes *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->setvalue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->setheader((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->on_pushButton_2_clicked(); break;
        case 5: _t->update(); break;
        case 6: _t->on_usetbl_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (tooltimes::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&tooltimes::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject tooltimes::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_tooltimes.data,
      qt_meta_data_tooltimes,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *tooltimes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tooltimes::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_tooltimes.stringdata0))
        return static_cast<void*>(const_cast< tooltimes*>(this));
    return QDialog::qt_metacast(_clname);
}

int tooltimes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void tooltimes::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
