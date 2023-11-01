/****************************************************************************
** Meta object code from reading C++ file 'Xwidgets.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Xwidgets.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Xwidgets.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DecKeypad_t {
    QByteArrayData data[5];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DecKeypad_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DecKeypad_t qt_meta_stringdata_DecKeypad = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DecKeypad"
QT_MOC_LITERAL(1, 10, 12), // "ensureSetVal"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "get_btn_sig"
QT_MOC_LITERAL(4, 36, 12) // "get_btn_sig2"

    },
    "DecKeypad\0ensureSetVal\0\0get_btn_sig\0"
    "get_btn_sig2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DecKeypad[] = {

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
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DecKeypad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DecKeypad *_t = static_cast<DecKeypad *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensureSetVal(); break;
        case 1: _t->get_btn_sig(); break;
        case 2: _t->get_btn_sig2(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject DecKeypad::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DecKeypad.data,
      qt_meta_data_DecKeypad,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DecKeypad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DecKeypad::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DecKeypad.stringdata0))
        return static_cast<void*>(const_cast< DecKeypad*>(this));
    return QDialog::qt_metacast(_clname);
}

int DecKeypad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_MySetVar_t {
    QByteArrayData data[4];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MySetVar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MySetVar_t qt_meta_stringdata_MySetVar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MySetVar"
QT_MOC_LITERAL(1, 9, 7), // "clicked"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 11) // "changeValue"

    },
    "MySetVar\0clicked\0\0changeValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MySetVar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,
       3,    1,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MySetVar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MySetVar *_t = static_cast<MySetVar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->changeValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->changeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MySetVar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetVar::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MySetVar::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetVar::changeValue)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MySetVar::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetVar::changeValue)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject MySetVar::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MySetVar.data,
      qt_meta_data_MySetVar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MySetVar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySetVar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MySetVar.stringdata0))
        return static_cast<void*>(const_cast< MySetVar*>(this));
    return QLabel::qt_metacast(_clname);
}

int MySetVar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void MySetVar::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MySetVar::changeValue(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MySetVar::changeValue(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
