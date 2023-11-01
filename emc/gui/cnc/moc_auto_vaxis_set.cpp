/****************************************************************************
** Meta object code from reading C++ file 'auto_vaxis_set.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/auto_vaxis_set.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'auto_vaxis_set.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_auto_vaxis_set_t {
    QByteArrayData data[10];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_auto_vaxis_set_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_auto_vaxis_set_t qt_meta_stringdata_auto_vaxis_set = {
    {
QT_MOC_LITERAL(0, 0, 14), // "auto_vaxis_set"
QT_MOC_LITERAL(1, 15, 19), // "set_pos_DownVXDepth"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "set_pos_DownVX"
QT_MOC_LITERAL(4, 51, 16), // "set_pos_DownVXYA"
QT_MOC_LITERAL(5, 68, 16), // "set_pos_DownVXYB"
QT_MOC_LITERAL(6, 85, 18), // "on_btn_VXB_clicked"
QT_MOC_LITERAL(7, 104, 7), // "checked"
QT_MOC_LITERAL(8, 112, 18), // "on_btn_VXA_clicked"
QT_MOC_LITERAL(9, 131, 8) // "set_Tool"

    },
    "auto_vaxis_set\0set_pos_DownVXDepth\0\0"
    "set_pos_DownVX\0set_pos_DownVXYA\0"
    "set_pos_DownVXYB\0on_btn_VXB_clicked\0"
    "checked\0on_btn_VXA_clicked\0set_Tool"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_auto_vaxis_set[] = {

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
       6,    1,   53,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x08 /* Private */,
       9,    0,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,

       0        // eod
};

void auto_vaxis_set::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto_vaxis_set *_t = static_cast<auto_vaxis_set *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->set_pos_DownVXDepth(); break;
        case 1: _t->set_pos_DownVX(); break;
        case 2: _t->set_pos_DownVXYA(); break;
        case 3: _t->set_pos_DownVXYB(); break;
        case 4: _t->on_btn_VXB_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_btn_VXA_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->set_Tool(); break;
        default: ;
        }
    }
}

const QMetaObject auto_vaxis_set::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_auto_vaxis_set.data,
      qt_meta_data_auto_vaxis_set,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *auto_vaxis_set::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *auto_vaxis_set::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_auto_vaxis_set.stringdata0))
        return static_cast<void*>(const_cast< auto_vaxis_set*>(this));
    return QWidget::qt_metacast(_clname);
}

int auto_vaxis_set::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
