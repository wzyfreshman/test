/****************************************************************************
** Meta object code from reading C++ file 'unit_datum.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_datum.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_datum.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Datum_t {
    QByteArrayData data[18];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Datum_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Datum_t qt_meta_stringdata_Datum = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Datum"
QT_MOC_LITERAL(1, 6, 8), // "send_msg"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 3), // "str"
QT_MOC_LITERAL(4, 20, 6), // "home_x"
QT_MOC_LITERAL(5, 27, 6), // "home_y"
QT_MOC_LITERAL(6, 34, 6), // "home_z"
QT_MOC_LITERAL(7, 41, 7), // "home_z2"
QT_MOC_LITERAL(8, 49, 8), // "unhome_x"
QT_MOC_LITERAL(9, 58, 8), // "unhome_y"
QT_MOC_LITERAL(10, 67, 8), // "unhome_z"
QT_MOC_LITERAL(11, 76, 9), // "unhome_z2"
QT_MOC_LITERAL(12, 86, 6), // "Export"
QT_MOC_LITERAL(13, 93, 6), // "Import"
QT_MOC_LITERAL(14, 100, 9), // "get_datum"
QT_MOC_LITERAL(15, 110, 4), // "axis"
QT_MOC_LITERAL(16, 115, 9), // "home_init"
QT_MOC_LITERAL(17, 125, 22) // "on_btn_modify_released"

    },
    "Datum\0send_msg\0\0str\0home_x\0home_y\0"
    "home_z\0home_z2\0unhome_x\0unhome_y\0"
    "unhome_z\0unhome_z2\0Export\0Import\0"
    "get_datum\0axis\0home_init\0"
    "on_btn_modify_released"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Datum[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   89,    2, 0x08 /* Private */,
       5,    0,   90,    2, 0x08 /* Private */,
       6,    0,   91,    2, 0x08 /* Private */,
       7,    0,   92,    2, 0x08 /* Private */,
       8,    0,   93,    2, 0x08 /* Private */,
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    1,   99,    2, 0x08 /* Private */,
      16,    0,  102,    2, 0x08 /* Private */,
      17,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Double, QMetaType::Int,   15,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Datum::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Datum *_t = static_cast<Datum *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->home_x(); break;
        case 2: _t->home_y(); break;
        case 3: _t->home_z(); break;
        case 4: _t->home_z2(); break;
        case 5: _t->unhome_x(); break;
        case 6: _t->unhome_y(); break;
        case 7: _t->unhome_z(); break;
        case 8: _t->unhome_z2(); break;
        case 9: _t->Export(); break;
        case 10: _t->Import(); break;
        case 11: { double _r = _t->get_datum((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 12: _t->home_init(); break;
        case 13: _t->on_btn_modify_released(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Datum::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Datum::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Datum::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Datum.data,
      qt_meta_data_Datum,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Datum::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Datum::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Datum.stringdata0))
        return static_cast<void*>(const_cast< Datum*>(this));
    return QDialog::qt_metacast(_clname);
}

int Datum::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Datum::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
