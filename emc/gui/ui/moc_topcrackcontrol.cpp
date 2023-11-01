/****************************************************************************
** Meta object code from reading C++ file 'topcrackcontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "topcrackcontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'topcrackcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TopCrackControl_t {
    QByteArrayData data[10];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TopCrackControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TopCrackControl_t qt_meta_stringdata_TopCrackControl = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TopCrackControl"
QT_MOC_LITERAL(1, 16, 21), // "on_btn_cancel_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 21), // "on_treeWidget_clicked"
QT_MOC_LITERAL(4, 61, 5), // "index"
QT_MOC_LITERAL(5, 67, 12), // "clickedCrack"
QT_MOC_LITERAL(6, 80, 16), // "refresh_selected"
QT_MOC_LITERAL(7, 97, 27), // "on_btn_turn_off_all_clicked"
QT_MOC_LITERAL(8, 125, 26), // "on_btn_turn_on_all_clicked"
QT_MOC_LITERAL(9, 152, 27) // "on_btn_turn_on_line_clicked"

    },
    "TopCrackControl\0on_btn_cancel_clicked\0"
    "\0on_treeWidget_clicked\0index\0clickedCrack\0"
    "refresh_selected\0on_btn_turn_off_all_clicked\0"
    "on_btn_turn_on_all_clicked\0"
    "on_btn_turn_on_line_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TopCrackControl[] = {

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
       3,    1,   50,    2, 0x08 /* Private */,
       5,    0,   53,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TopCrackControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TopCrackControl *_t = static_cast<TopCrackControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_cancel_clicked(); break;
        case 1: _t->on_treeWidget_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->clickedCrack(); break;
        case 3: _t->refresh_selected(); break;
        case 4: _t->on_btn_turn_off_all_clicked(); break;
        case 5: _t->on_btn_turn_on_all_clicked(); break;
        case 6: _t->on_btn_turn_on_line_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject TopCrackControl::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TopCrackControl.data,
      qt_meta_data_TopCrackControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TopCrackControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TopCrackControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TopCrackControl.stringdata0))
        return static_cast<void*>(const_cast< TopCrackControl*>(this));
    return QDialog::qt_metacast(_clname);
}

int TopCrackControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
