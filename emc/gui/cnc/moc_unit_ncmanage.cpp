/****************************************************************************
** Meta object code from reading C++ file 'unit_ncmanage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_ncmanage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_ncmanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_unit_ncmanage_t {
    QByteArrayData data[10];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_ncmanage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_ncmanage_t qt_meta_stringdata_unit_ncmanage = {
    {
QT_MOC_LITERAL(0, 0, 13), // "unit_ncmanage"
QT_MOC_LITERAL(1, 14, 8), // "fileEdit"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "fileUnload"
QT_MOC_LITERAL(4, 35, 8), // "fileOpen"
QT_MOC_LITERAL(5, 44, 10), // "jumpToLine"
QT_MOC_LITERAL(6, 55, 7), // "showMsg"
QT_MOC_LITERAL(7, 63, 10), // "reloadfile"
QT_MOC_LITERAL(8, 74, 4), // "file"
QT_MOC_LITERAL(9, 79, 19) // "on_btn_open_clicked"

    },
    "unit_ncmanage\0fileEdit\0\0fileUnload\0"
    "fileOpen\0jumpToLine\0showMsg\0reloadfile\0"
    "file\0on_btn_open_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_ncmanage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    0,   52,    2, 0x06 /* Public */,
       4,    1,   53,    2, 0x06 /* Public */,
       5,    1,   56,    2, 0x06 /* Public */,
       6,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   64,    2, 0x0a /* Public */,
       9,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void unit_ncmanage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_ncmanage *_t = static_cast<unit_ncmanage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->fileUnload(); break;
        case 2: _t->fileOpen((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->jumpToLine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->reloadfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->on_btn_open_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_ncmanage::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_ncmanage::fileEdit)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (unit_ncmanage::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_ncmanage::fileUnload)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (unit_ncmanage::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_ncmanage::fileOpen)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (unit_ncmanage::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_ncmanage::jumpToLine)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (unit_ncmanage::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_ncmanage::showMsg)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject unit_ncmanage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_unit_ncmanage.data,
      qt_meta_data_unit_ncmanage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_ncmanage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_ncmanage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_ncmanage.stringdata0))
        return static_cast<void*>(const_cast< unit_ncmanage*>(this));
    return QWidget::qt_metacast(_clname);
}

int unit_ncmanage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void unit_ncmanage::fileEdit(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void unit_ncmanage::fileUnload()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void unit_ncmanage::fileOpen(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void unit_ncmanage::jumpToLine(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void unit_ncmanage::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
