/****************************************************************************
** Meta object code from reading C++ file 'unit_homedialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_homedialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_homedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_unit_homedialog_t {
    QByteArrayData data[22];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_homedialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_homedialog_t qt_meta_stringdata_unit_homedialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "unit_homedialog"
QT_MOC_LITERAL(1, 16, 8), // "send_msg"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "str"
QT_MOC_LITERAL(4, 30, 8), // "home_all"
QT_MOC_LITERAL(5, 39, 6), // "home_x"
QT_MOC_LITERAL(6, 46, 6), // "home_y"
QT_MOC_LITERAL(7, 53, 6), // "home_z"
QT_MOC_LITERAL(8, 60, 6), // "home_a"
QT_MOC_LITERAL(9, 67, 6), // "home_b"
QT_MOC_LITERAL(10, 74, 6), // "home_c"
QT_MOC_LITERAL(11, 81, 9), // "home_quit"
QT_MOC_LITERAL(12, 91, 11), // "home_direct"
QT_MOC_LITERAL(13, 103, 11), // "home_update"
QT_MOC_LITERAL(14, 115, 9), // "home_init"
QT_MOC_LITERAL(15, 125, 8), // "unhome_x"
QT_MOC_LITERAL(16, 134, 8), // "unhome_y"
QT_MOC_LITERAL(17, 143, 8), // "unhome_z"
QT_MOC_LITERAL(18, 152, 8), // "unhome_a"
QT_MOC_LITERAL(19, 161, 8), // "unhome_b"
QT_MOC_LITERAL(20, 170, 8), // "unhome_c"
QT_MOC_LITERAL(21, 179, 19) // "on_btn_stop_pressed"

    },
    "unit_homedialog\0send_msg\0\0str\0home_all\0"
    "home_x\0home_y\0home_z\0home_a\0home_b\0"
    "home_c\0home_quit\0home_direct\0home_update\0"
    "home_init\0unhome_x\0unhome_y\0unhome_z\0"
    "unhome_a\0unhome_b\0unhome_c\0"
    "on_btn_stop_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_homedialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  114,    2, 0x0a /* Public */,
       5,    0,  115,    2, 0x0a /* Public */,
       6,    0,  116,    2, 0x0a /* Public */,
       7,    0,  117,    2, 0x0a /* Public */,
       8,    0,  118,    2, 0x0a /* Public */,
       9,    0,  119,    2, 0x0a /* Public */,
      10,    0,  120,    2, 0x0a /* Public */,
      11,    0,  121,    2, 0x0a /* Public */,
      12,    0,  122,    2, 0x0a /* Public */,
      13,    0,  123,    2, 0x0a /* Public */,
      14,    0,  124,    2, 0x0a /* Public */,
      15,    0,  125,    2, 0x0a /* Public */,
      16,    0,  126,    2, 0x0a /* Public */,
      17,    0,  127,    2, 0x0a /* Public */,
      18,    0,  128,    2, 0x0a /* Public */,
      19,    0,  129,    2, 0x0a /* Public */,
      20,    0,  130,    2, 0x0a /* Public */,
      21,    0,  131,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void unit_homedialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_homedialog *_t = static_cast<unit_homedialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->home_all(); break;
        case 2: _t->home_x(); break;
        case 3: _t->home_y(); break;
        case 4: _t->home_z(); break;
        case 5: _t->home_a(); break;
        case 6: _t->home_b(); break;
        case 7: _t->home_c(); break;
        case 8: _t->home_quit(); break;
        case 9: _t->home_direct(); break;
        case 10: _t->home_update(); break;
        case 11: _t->home_init(); break;
        case 12: _t->unhome_x(); break;
        case 13: _t->unhome_y(); break;
        case 14: _t->unhome_z(); break;
        case 15: _t->unhome_a(); break;
        case 16: _t->unhome_b(); break;
        case 17: _t->unhome_c(); break;
        case 18: _t->on_btn_stop_pressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_homedialog::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_homedialog::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject unit_homedialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_unit_homedialog.data,
      qt_meta_data_unit_homedialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_homedialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_homedialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_homedialog.stringdata0))
        return static_cast<void*>(const_cast< unit_homedialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int unit_homedialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void unit_homedialog::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
