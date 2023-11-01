/****************************************************************************
** Meta object code from reading C++ file 'rwmotor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/rwmotor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rwmotor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RWMOTOR_t {
    QByteArrayData data[4];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RWMOTOR_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RWMOTOR_t qt_meta_stringdata_RWMOTOR = {
    {
QT_MOC_LITERAL(0, 0, 7), // "RWMOTOR"
QT_MOC_LITERAL(1, 8, 25), // "on_partable_doubleClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5) // "index"

    },
    "RWMOTOR\0on_partable_doubleClicked\0\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RWMOTOR[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    3,

       0        // eod
};

void RWMOTOR::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RWMOTOR *_t = static_cast<RWMOTOR *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_partable_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject RWMOTOR::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RWMOTOR.data,
      qt_meta_data_RWMOTOR,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RWMOTOR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RWMOTOR::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RWMOTOR.stringdata0))
        return static_cast<void*>(const_cast< RWMOTOR*>(this));
    return QWidget::qt_metacast(_clname);
}

int RWMOTOR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MULTIMOTOR_t {
    QByteArrayData data[5];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MULTIMOTOR_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MULTIMOTOR_t qt_meta_stringdata_MULTIMOTOR = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MULTIMOTOR"
QT_MOC_LITERAL(1, 11, 6), // "ensure"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 4), // "quit"
QT_MOC_LITERAL(4, 24, 12) // "initAxisData"

    },
    "MULTIMOTOR\0ensure\0\0quit\0initAxisData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MULTIMOTOR[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MULTIMOTOR::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MULTIMOTOR *_t = static_cast<MULTIMOTOR *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensure(); break;
        case 1: _t->quit(); break;
        case 2: _t->initAxisData(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MULTIMOTOR::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MULTIMOTOR.data,
      qt_meta_data_MULTIMOTOR,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MULTIMOTOR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MULTIMOTOR::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MULTIMOTOR.stringdata0))
        return static_cast<void*>(const_cast< MULTIMOTOR*>(this));
    return QDialog::qt_metacast(_clname);
}

int MULTIMOTOR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
