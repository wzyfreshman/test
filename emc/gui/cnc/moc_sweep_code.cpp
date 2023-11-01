/****************************************************************************
** Meta object code from reading C++ file 'sweep_code.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/sweep_code.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sweep_code.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sweep_code_t {
    QByteArrayData data[10];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sweep_code_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sweep_code_t qt_meta_stringdata_sweep_code = {
    {
QT_MOC_LITERAL(0, 0, 10), // "sweep_code"
QT_MOC_LITERAL(1, 11, 12), // "get_keyvalue"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "count"
QT_MOC_LITERAL(4, 31, 1), // "x"
QT_MOC_LITERAL(5, 33, 1), // "y"
QT_MOC_LITERAL(6, 35, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(7, 57, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(8, 81, 11), // "set_count_x"
QT_MOC_LITERAL(9, 93, 11) // "set_count_y"

    },
    "sweep_code\0get_keyvalue\0\0count\0x\0y\0"
    "on_pushButton_clicked\0on_pushButton_2_clicked\0"
    "set_count_x\0set_count_y"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sweep_code[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x0a /* Public */,
       6,    0,   46,    2, 0x08 /* Private */,
       7,    0,   47,    2, 0x08 /* Private */,
       8,    0,   48,    2, 0x08 /* Private */,
       9,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sweep_code::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        sweep_code *_t = static_cast<sweep_code *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->get_keyvalue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 1: _t->on_pushButton_clicked(); break;
        case 2: _t->on_pushButton_2_clicked(); break;
        case 3: _t->set_count_x(); break;
        case 4: _t->set_count_y(); break;
        default: ;
        }
    }
}

const QMetaObject sweep_code::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_sweep_code.data,
      qt_meta_data_sweep_code,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sweep_code::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sweep_code::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sweep_code.stringdata0))
        return static_cast<void*>(const_cast< sweep_code*>(this));
    return QDialog::qt_metacast(_clname);
}

int sweep_code::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
