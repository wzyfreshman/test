/****************************************************************************
** Meta object code from reading C++ file 'parameter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/parameter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parameter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Parameter_t {
    QByteArrayData data[10];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Parameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Parameter_t qt_meta_stringdata_Parameter = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Parameter"
QT_MOC_LITERAL(1, 10, 7), // "sendMsg"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 9), // "addwidget"
QT_MOC_LITERAL(4, 29, 10), // "addconnect"
QT_MOC_LITERAL(5, 40, 12), // "set_maxlimit"
QT_MOC_LITERAL(6, 53, 12), // "set_minlimit"
QT_MOC_LITERAL(7, 66, 10), // "set_jogacc"
QT_MOC_LITERAL(8, 77, 26), // "on_btn_axis_unhome_clicked"
QT_MOC_LITERAL(9, 104, 29) // "on_btn_set_axis_datum_clicked"

    },
    "Parameter\0sendMsg\0\0addwidget\0addconnect\0"
    "set_maxlimit\0set_minlimit\0set_jogacc\0"
    "on_btn_axis_unhome_clicked\0"
    "on_btn_set_axis_datum_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Parameter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   59,    2, 0x08 /* Private */,
       4,    0,   60,    2, 0x08 /* Private */,
       5,    0,   61,    2, 0x08 /* Private */,
       6,    0,   62,    2, 0x08 /* Private */,
       7,    0,   63,    2, 0x08 /* Private */,
       8,    0,   64,    2, 0x08 /* Private */,
       9,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Parameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Parameter *_t = static_cast<Parameter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->addwidget(); break;
        case 2: _t->addconnect(); break;
        case 3: _t->set_maxlimit(); break;
        case 4: _t->set_minlimit(); break;
        case 5: _t->set_jogacc(); break;
        case 6: _t->on_btn_axis_unhome_clicked(); break;
        case 7: _t->on_btn_set_axis_datum_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Parameter::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Parameter::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Parameter::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Parameter.data,
      qt_meta_data_Parameter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Parameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Parameter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Parameter.stringdata0))
        return static_cast<void*>(const_cast< Parameter*>(this));
    return QDialog::qt_metacast(_clname);
}

int Parameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Parameter::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_AxisParameter_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AxisParameter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AxisParameter_t qt_meta_stringdata_AxisParameter = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AxisParameter"
QT_MOC_LITERAL(1, 14, 7), // "sendMsg"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "AxisParameter\0sendMsg\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AxisParameter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

       0        // eod
};

void AxisParameter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AxisParameter *_t = static_cast<AxisParameter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AxisParameter::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AxisParameter::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AxisParameter::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AxisParameter.data,
      qt_meta_data_AxisParameter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AxisParameter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AxisParameter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AxisParameter.stringdata0))
        return static_cast<void*>(const_cast< AxisParameter*>(this));
    return QDialog::qt_metacast(_clname);
}

int AxisParameter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void AxisParameter::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
