/****************************************************************************
** Meta object code from reading C++ file 'mode_selection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/mode_selection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mode_selection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Mode_selection_t {
    QByteArrayData data[9];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Mode_selection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Mode_selection_t qt_meta_stringdata_Mode_selection = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Mode_selection"
QT_MOC_LITERAL(1, 15, 16), // "on_mode1_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 16), // "on_mode2_clicked"
QT_MOC_LITERAL(4, 50, 16), // "on_mode3_clicked"
QT_MOC_LITERAL(5, 67, 16), // "on_mode4_clicked"
QT_MOC_LITERAL(6, 84, 16), // "on_mode5_clicked"
QT_MOC_LITERAL(7, 101, 21), // "on_btn_cancel_clicked"
QT_MOC_LITERAL(8, 123, 20) // "on_btn_drill_clicked"

    },
    "Mode_selection\0on_mode1_clicked\0\0"
    "on_mode2_clicked\0on_mode3_clicked\0"
    "on_mode4_clicked\0on_mode5_clicked\0"
    "on_btn_cancel_clicked\0on_btn_drill_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mode_selection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Mode_selection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Mode_selection *_t = static_cast<Mode_selection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_mode1_clicked(); break;
        case 1: _t->on_mode2_clicked(); break;
        case 2: _t->on_mode3_clicked(); break;
        case 3: _t->on_mode4_clicked(); break;
        case 4: _t->on_mode5_clicked(); break;
        case 5: _t->on_btn_cancel_clicked(); break;
        case 6: _t->on_btn_drill_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Mode_selection::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Mode_selection.data,
      qt_meta_data_Mode_selection,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Mode_selection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mode_selection::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Mode_selection.stringdata0))
        return static_cast<void*>(const_cast< Mode_selection*>(this));
    return QDialog::qt_metacast(_clname);
}

int Mode_selection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
