/****************************************************************************
** Meta object code from reading C++ file 'set_underaxisoffset.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/set_underaxisoffset.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'set_underaxisoffset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_set_underaxisoffset_t {
    QByteArrayData data[9];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_set_underaxisoffset_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_set_underaxisoffset_t qt_meta_stringdata_set_underaxisoffset = {
    {
QT_MOC_LITERAL(0, 0, 19), // "set_underaxisoffset"
QT_MOC_LITERAL(1, 20, 8), // "show_abs"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 10), // "updatecoor"
QT_MOC_LITERAL(4, 41, 14), // "set_hor_offset"
QT_MOC_LITERAL(5, 56, 12), // "set_y_offset"
QT_MOC_LITERAL(6, 69, 12), // "set_z_offset"
QT_MOC_LITERAL(7, 82, 13), // "set_va_offset"
QT_MOC_LITERAL(8, 96, 13) // "set_vb_offset"

    },
    "set_underaxisoffset\0show_abs\0\0updatecoor\0"
    "set_hor_offset\0set_y_offset\0set_z_offset\0"
    "set_va_offset\0set_vb_offset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_set_underaxisoffset[] = {

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
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
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

void set_underaxisoffset::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        set_underaxisoffset *_t = static_cast<set_underaxisoffset *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_abs(); break;
        case 1: _t->updatecoor(); break;
        case 2: _t->set_hor_offset(); break;
        case 3: _t->set_y_offset(); break;
        case 4: _t->set_z_offset(); break;
        case 5: _t->set_va_offset(); break;
        case 6: _t->set_vb_offset(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject set_underaxisoffset::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_set_underaxisoffset.data,
      qt_meta_data_set_underaxisoffset,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *set_underaxisoffset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *set_underaxisoffset::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_set_underaxisoffset.stringdata0))
        return static_cast<void*>(const_cast< set_underaxisoffset*>(this));
    return QWidget::qt_metacast(_clname);
}

int set_underaxisoffset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
