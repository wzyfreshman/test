/****************************************************************************
** Meta object code from reading C++ file 'toolparameters.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "toolparameters.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolparameters.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ToolParameters_t {
    QByteArrayData data[20];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolParameters_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolParameters_t qt_meta_stringdata_ToolParameters = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ToolParameters"
QT_MOC_LITERAL(1, 15, 8), // "show_msg"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "set_spindle"
QT_MOC_LITERAL(4, 37, 2), // "id"
QT_MOC_LITERAL(5, 40, 12), // "set_z_offset"
QT_MOC_LITERAL(6, 53, 13), // "set_2offset_x"
QT_MOC_LITERAL(7, 67, 13), // "set_2offset_y"
QT_MOC_LITERAL(8, 81, 16), // "set_spindle_x1_2"
QT_MOC_LITERAL(9, 98, 16), // "set_spindle_y1_2"
QT_MOC_LITERAL(10, 115, 16), // "set_spindle_x1_3"
QT_MOC_LITERAL(11, 132, 16), // "set_spindle_y1_3"
QT_MOC_LITERAL(12, 149, 15), // "set_alloffset_x"
QT_MOC_LITERAL(13, 165, 15), // "set_alloffset_y"
QT_MOC_LITERAL(14, 181, 20), // "on_btn_close_clicked"
QT_MOC_LITERAL(15, 202, 11), // "ShowXOffset"
QT_MOC_LITERAL(16, 214, 3), // "num"
QT_MOC_LITERAL(17, 218, 11), // "ShowYOffset"
QT_MOC_LITERAL(18, 230, 11), // "ShowZOffset"
QT_MOC_LITERAL(19, 242, 19) // "on_checkBox_clicked"

    },
    "ToolParameters\0show_msg\0\0set_spindle\0"
    "id\0set_z_offset\0set_2offset_x\0"
    "set_2offset_y\0set_spindle_x1_2\0"
    "set_spindle_y1_2\0set_spindle_x1_3\0"
    "set_spindle_y1_3\0set_alloffset_x\0"
    "set_alloffset_y\0on_btn_close_clicked\0"
    "ShowXOffset\0num\0ShowYOffset\0ShowZOffset\0"
    "on_checkBox_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolParameters[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   99,    2, 0x08 /* Private */,
       5,    1,  102,    2, 0x08 /* Private */,
       6,    0,  105,    2, 0x08 /* Private */,
       7,    0,  106,    2, 0x08 /* Private */,
       8,    0,  107,    2, 0x08 /* Private */,
       9,    0,  108,    2, 0x08 /* Private */,
      10,    0,  109,    2, 0x08 /* Private */,
      11,    0,  110,    2, 0x08 /* Private */,
      12,    0,  111,    2, 0x08 /* Private */,
      13,    0,  112,    2, 0x08 /* Private */,
      14,    0,  113,    2, 0x08 /* Private */,
      15,    1,  114,    2, 0x08 /* Private */,
      17,    1,  117,    2, 0x08 /* Private */,
      18,    1,  120,    2, 0x08 /* Private */,
      19,    0,  123,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,

       0        // eod
};

void ToolParameters::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolParameters *_t = static_cast<ToolParameters *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->set_spindle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->set_z_offset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->set_2offset_x(); break;
        case 4: _t->set_2offset_y(); break;
        case 5: _t->set_spindle_x1_2(); break;
        case 6: _t->set_spindle_y1_2(); break;
        case 7: _t->set_spindle_x1_3(); break;
        case 8: _t->set_spindle_y1_3(); break;
        case 9: _t->set_alloffset_x(); break;
        case 10: _t->set_alloffset_y(); break;
        case 11: _t->on_btn_close_clicked(); break;
        case 12: _t->ShowXOffset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->ShowYOffset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->ShowZOffset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_checkBox_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ToolParameters::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ToolParameters::show_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ToolParameters::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ToolParameters.data,
      qt_meta_data_ToolParameters,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ToolParameters::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolParameters::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ToolParameters.stringdata0))
        return static_cast<void*>(const_cast< ToolParameters*>(this));
    return QDialog::qt_metacast(_clname);
}

int ToolParameters::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void ToolParameters::show_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
