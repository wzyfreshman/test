/****************************************************************************
** Meta object code from reading C++ file 'product_id.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "newui/product_id.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'product_id.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Product_ID_t {
    QByteArrayData data[6];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Product_ID_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Product_ID_t qt_meta_stringdata_Product_ID = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Product_ID"
QT_MOC_LITERAL(1, 11, 15), // "on_Save_clicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 17), // "on_Cancel_clicked"
QT_MOC_LITERAL(4, 46, 17), // "on_Modify_clicked"
QT_MOC_LITERAL(5, 64, 7) // "checked"

    },
    "Product_ID\0on_Save_clicked\0\0"
    "on_Cancel_clicked\0on_Modify_clicked\0"
    "checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Product_ID[] = {

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
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void Product_ID::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Product_ID *_t = static_cast<Product_ID *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_Save_clicked(); break;
        case 1: _t->on_Cancel_clicked(); break;
        case 2: _t->on_Modify_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Product_ID::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Product_ID.data,
      qt_meta_data_Product_ID,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Product_ID::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Product_ID::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Product_ID.stringdata0))
        return static_cast<void*>(const_cast< Product_ID*>(this));
    return QDialog::qt_metacast(_clname);
}

int Product_ID::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
