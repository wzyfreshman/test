/****************************************************************************
** Meta object code from reading C++ file 'vxaxis_manual_move.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/vxaxis_manual_move.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vxaxis_manual_move.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_vxaxis_manual_move_t {
    QByteArrayData data[18];
    char stringdata0[299];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_vxaxis_manual_move_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_vxaxis_manual_move_t qt_meta_stringdata_vxaxis_manual_move = {
    {
QT_MOC_LITERAL(0, 0, 18), // "vxaxis_manual_move"
QT_MOC_LITERAL(1, 19, 23), // "axis_vxver_plus_pressed"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 22), // "axis_vxver_dec_pressed"
QT_MOC_LITERAL(4, 67, 23), // "axis_vxhor_plus_pressed"
QT_MOC_LITERAL(5, 91, 22), // "axis_vxhor_dec_pressed"
QT_MOC_LITERAL(6, 114, 12), // "stopAxisMove"
QT_MOC_LITERAL(7, 127, 15), // "plus_safe_check"
QT_MOC_LITERAL(8, 143, 1), // "i"
QT_MOC_LITERAL(9, 145, 1), // "j"
QT_MOC_LITERAL(10, 147, 21), // "on_btn_vxfeed_clicked"
QT_MOC_LITERAL(11, 169, 7), // "checked"
QT_MOC_LITERAL(12, 177, 25), // "on_btn_vxdrilling_clicked"
QT_MOC_LITERAL(13, 203, 21), // "on_btn_motora_clicked"
QT_MOC_LITERAL(14, 225, 21), // "on_btn_motorb_clicked"
QT_MOC_LITERAL(15, 247, 10), // "checkValid"
QT_MOC_LITERAL(16, 258, 20), // "axis_vy_plus_pressed"
QT_MOC_LITERAL(17, 279, 19) // "axis_vy_dec_pressed"

    },
    "vxaxis_manual_move\0axis_vxver_plus_pressed\0"
    "\0axis_vxver_dec_pressed\0axis_vxhor_plus_pressed\0"
    "axis_vxhor_dec_pressed\0stopAxisMove\0"
    "plus_safe_check\0i\0j\0on_btn_vxfeed_clicked\0"
    "checked\0on_btn_vxdrilling_clicked\0"
    "on_btn_motora_clicked\0on_btn_motorb_clicked\0"
    "checkValid\0axis_vy_plus_pressed\0"
    "axis_vy_dec_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_vxaxis_manual_move[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    2,   84,    2, 0x08 /* Private */,
      10,    1,   89,    2, 0x08 /* Private */,
      12,    0,   92,    2, 0x08 /* Private */,
      13,    0,   93,    2, 0x08 /* Private */,
      14,    0,   94,    2, 0x08 /* Private */,
      15,    0,   95,    2, 0x08 /* Private */,
      16,    0,   96,    2, 0x08 /* Private */,
      17,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Double, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void vxaxis_manual_move::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        vxaxis_manual_move *_t = static_cast<vxaxis_manual_move *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->axis_vxver_plus_pressed(); break;
        case 1: _t->axis_vxver_dec_pressed(); break;
        case 2: _t->axis_vxhor_plus_pressed(); break;
        case 3: _t->axis_vxhor_dec_pressed(); break;
        case 4: _t->stopAxisMove(); break;
        case 5: { double _r = _t->plus_safe_check((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 6: _t->on_btn_vxfeed_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_btn_vxdrilling_clicked(); break;
        case 8: _t->on_btn_motora_clicked(); break;
        case 9: _t->on_btn_motorb_clicked(); break;
        case 10: { bool _r = _t->checkValid();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: _t->axis_vy_plus_pressed(); break;
        case 12: _t->axis_vy_dec_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject vxaxis_manual_move::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_vxaxis_manual_move.data,
      qt_meta_data_vxaxis_manual_move,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *vxaxis_manual_move::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *vxaxis_manual_move::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_vxaxis_manual_move.stringdata0))
        return static_cast<void*>(const_cast< vxaxis_manual_move*>(this));
    return QWidget::qt_metacast(_clname);
}

int vxaxis_manual_move::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
