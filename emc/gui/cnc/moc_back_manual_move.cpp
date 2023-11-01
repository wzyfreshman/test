/****************************************************************************
** Meta object code from reading C++ file 'back_manual_move.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/back_manual_move.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'back_manual_move.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_back_manual_move_t {
    QByteArrayData data[14];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_back_manual_move_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_back_manual_move_t qt_meta_stringdata_back_manual_move = {
    {
QT_MOC_LITERAL(0, 0, 16), // "back_manual_move"
QT_MOC_LITERAL(1, 17, 10), // "updateback"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 22), // "axis_back_plus_pressed"
QT_MOC_LITERAL(4, 52, 21), // "axis_back_dec_pressed"
QT_MOC_LITERAL(5, 74, 12), // "stopAxisMove"
QT_MOC_LITERAL(6, 87, 26), // "on_btn_backmotoron_clicked"
QT_MOC_LITERAL(7, 114, 24), // "on_btn_backdrill_clicked"
QT_MOC_LITERAL(8, 139, 21), // "on_btn_downio_clicked"
QT_MOC_LITERAL(9, 161, 22), // "on_btn_enterio_clicked"
QT_MOC_LITERAL(10, 184, 15), // "plus_safe_check"
QT_MOC_LITERAL(11, 200, 1), // "i"
QT_MOC_LITERAL(12, 202, 1), // "j"
QT_MOC_LITERAL(13, 204, 10) // "checkValid"

    },
    "back_manual_move\0updateback\0\0"
    "axis_back_plus_pressed\0axis_back_dec_pressed\0"
    "stopAxisMove\0on_btn_backmotoron_clicked\0"
    "on_btn_backdrill_clicked\0on_btn_downio_clicked\0"
    "on_btn_enterio_clicked\0plus_safe_check\0"
    "i\0j\0checkValid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_back_manual_move[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    2,   72,    2, 0x08 /* Private */,
      13,    0,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Double, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Bool,

       0        // eod
};

void back_manual_move::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        back_manual_move *_t = static_cast<back_manual_move *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateback(); break;
        case 1: _t->axis_back_plus_pressed(); break;
        case 2: _t->axis_back_dec_pressed(); break;
        case 3: _t->stopAxisMove(); break;
        case 4: _t->on_btn_backmotoron_clicked(); break;
        case 5: _t->on_btn_backdrill_clicked(); break;
        case 6: _t->on_btn_downio_clicked(); break;
        case 7: _t->on_btn_enterio_clicked(); break;
        case 8: { double _r = _t->plus_safe_check((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->checkValid();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject back_manual_move::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_back_manual_move.data,
      qt_meta_data_back_manual_move,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *back_manual_move::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *back_manual_move::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_back_manual_move.stringdata0))
        return static_cast<void*>(const_cast< back_manual_move*>(this));
    return QWidget::qt_metacast(_clname);
}

int back_manual_move::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
