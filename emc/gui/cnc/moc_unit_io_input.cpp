/****************************************************************************
** Meta object code from reading C++ file 'unit_io_input.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_io_input.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_io_input.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_tableButton_t {
    QByteArrayData data[4];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tableButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tableButton_t qt_meta_stringdata_tableButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "tableButton"
QT_MOC_LITERAL(1, 12, 11), // "sendClicked"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10) // "setClicked"

    },
    "tableButton\0sendClicked\0\0setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tableButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void tableButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        tableButton *_t = static_cast<tableButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (tableButton::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&tableButton::sendClicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject tableButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_tableButton.data,
      qt_meta_data_tableButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *tableButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tableButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_tableButton.stringdata0))
        return static_cast<void*>(const_cast< tableButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int tableButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void tableButton::sendClicked(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_IO_input_t {
    QByteArrayData data[9];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IO_input_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IO_input_t qt_meta_stringdata_IO_input = {
    {
QT_MOC_LITERAL(0, 0, 8), // "IO_input"
QT_MOC_LITERAL(1, 9, 10), // "refresh_io"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 9), // "io_update"
QT_MOC_LITERAL(4, 31, 7), // "io_init"
QT_MOC_LITERAL(5, 39, 11), // "getSimClick"
QT_MOC_LITERAL(6, 51, 9), // "IOControl"
QT_MOC_LITERAL(7, 61, 14), // "init_io_status"
QT_MOC_LITERAL(8, 76, 7) // "needpor"

    },
    "IO_input\0refresh_io\0\0io_update\0io_init\0"
    "getSimClick\0IOControl\0init_io_status\0"
    "needpor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IO_input[] = {

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
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    2,   52,    2, 0x0a /* Public */,
       6,    0,   57,    2, 0x0a /* Public */,
       7,    1,   58,    2, 0x0a /* Public */,
       7,    0,   61,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

       0        // eod
};

void IO_input::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IO_input *_t = static_cast<IO_input *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refresh_io(); break;
        case 1: _t->io_update(); break;
        case 2: _t->io_init(); break;
        case 3: _t->getSimClick((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->IOControl(); break;
        case 5: _t->init_io_status((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->init_io_status(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IO_input::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IO_input::refresh_io)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject IO_input::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_IO_input.data,
      qt_meta_data_IO_input,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IO_input::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IO_input::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IO_input.stringdata0))
        return static_cast<void*>(const_cast< IO_input*>(this));
    return QDialog::qt_metacast(_clname);
}

int IO_input::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void IO_input::refresh_io()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
