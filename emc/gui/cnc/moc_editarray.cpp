/****************************************************************************
** Meta object code from reading C++ file 'editarray.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/editarray.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editarray.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mychk_t {
    QByteArrayData data[7];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mychk_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mychk_t qt_meta_stringdata_mychk = {
    {
QT_MOC_LITERAL(0, 0, 5), // "mychk"
QT_MOC_LITERAL(1, 6, 8), // "getclick"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 8), // "clicked1"
QT_MOC_LITERAL(4, 25, 8), // "clicked2"
QT_MOC_LITERAL(5, 34, 10), // "chkclicked"
QT_MOC_LITERAL(6, 45, 6) // "mychk*"

    },
    "mychk\0getclick\0\0clicked1\0clicked2\0"
    "chkclicked\0mychk*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mychk[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    1,   37,    2, 0x06 /* Public */,
       4,    1,   40,    2, 0x06 /* Public */,
       5,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 6,    2,

       0        // eod
};

void mychk::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mychk *_t = static_cast<mychk *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getclick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->clicked1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->clicked2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->chkclicked((*reinterpret_cast< mychk*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mychk* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (mychk::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mychk::getclick)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (mychk::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mychk::clicked1)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (mychk::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mychk::clicked2)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (mychk::*_t)(mychk * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mychk::chkclicked)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject mychk::staticMetaObject = {
    { &QCheckBox::staticMetaObject, qt_meta_stringdata_mychk.data,
      qt_meta_data_mychk,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mychk::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mychk::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mychk.stringdata0))
        return static_cast<void*>(const_cast< mychk*>(this));
    return QCheckBox::qt_metacast(_clname);
}

int mychk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCheckBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void mychk::getclick(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void mychk::clicked1(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void mychk::clicked2(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void mychk::chkclicked(mychk * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_EditArray_t {
    QByteArrayData data[8];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EditArray_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EditArray_t qt_meta_stringdata_EditArray = {
    {
QT_MOC_LITERAL(0, 0, 9), // "EditArray"
QT_MOC_LITERAL(1, 10, 8), // "arraynum"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 8), // "setcheck"
QT_MOC_LITERAL(4, 29, 6), // "mychk*"
QT_MOC_LITERAL(5, 36, 11), // "changeValue"
QT_MOC_LITERAL(6, 48, 10), // "reselected"
QT_MOC_LITERAL(7, 59, 8) // "updateUI"

    },
    "EditArray\0arraynum\0\0setcheck\0mychk*\0"
    "changeValue\0reselected\0updateUI"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EditArray[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    2,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   47,    2, 0x0a /* Public */,
       6,    0,   50,    2, 0x0a /* Public */,
       7,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void EditArray::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditArray *_t = static_cast<EditArray *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->arraynum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setcheck((*reinterpret_cast< mychk*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->changeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->reselected(); break;
        case 4: _t->updateUI(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mychk* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EditArray::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EditArray::arraynum)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (EditArray::*_t)(mychk * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EditArray::setcheck)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject EditArray::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EditArray.data,
      qt_meta_data_EditArray,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EditArray::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditArray::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EditArray.stringdata0))
        return static_cast<void*>(const_cast< EditArray*>(this));
    return QDialog::qt_metacast(_clname);
}

int EditArray::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void EditArray::arraynum(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EditArray::setcheck(mychk * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
