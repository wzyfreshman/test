/****************************************************************************
** Meta object code from reading C++ file 'permission.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/permission.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'permission.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Permission_t {
    QByteArrayData data[6];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Permission_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Permission_t qt_meta_stringdata_Permission = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Permission"
QT_MOC_LITERAL(1, 11, 21), // "on_btn_debug_released"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 19), // "on_btn_dev_released"
QT_MOC_LITERAL(4, 54, 19), // "on_btn_quit_clicked"
QT_MOC_LITERAL(5, 74, 20) // "on_btn_oper_released"

    },
    "Permission\0on_btn_debug_released\0\0"
    "on_btn_dev_released\0on_btn_quit_clicked\0"
    "on_btn_oper_released"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Permission[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Permission::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Permission *_t = static_cast<Permission *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_debug_released(); break;
        case 1: _t->on_btn_dev_released(); break;
        case 2: _t->on_btn_quit_clicked(); break;
        case 3: _t->on_btn_oper_released(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Permission::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Permission.data,
      qt_meta_data_Permission,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Permission::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Permission::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Permission.stringdata0))
        return static_cast<void*>(const_cast< Permission*>(this));
    return QDialog::qt_metacast(_clname);
}

int Permission::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
