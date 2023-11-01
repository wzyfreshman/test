/****************************************************************************
** Meta object code from reading C++ file 'formdownstrut.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "newui/formdownstrut.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formdownstrut.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FormDownStrut_t {
    QByteArrayData data[8];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FormDownStrut_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FormDownStrut_t qt_meta_stringdata_FormDownStrut = {
    {
QT_MOC_LITERAL(0, 0, 13), // "FormDownStrut"
QT_MOC_LITERAL(1, 14, 23), // "axis_vxhor_plus_pressed"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 22), // "axis_vxhor_dec_pressed"
QT_MOC_LITERAL(4, 62, 12), // "stopAxisMove"
QT_MOC_LITERAL(5, 75, 15), // "plus_safe_check"
QT_MOC_LITERAL(6, 91, 1), // "i"
QT_MOC_LITERAL(7, 93, 1) // "j"

    },
    "FormDownStrut\0axis_vxhor_plus_pressed\0"
    "\0axis_vxhor_dec_pressed\0stopAxisMove\0"
    "plus_safe_check\0i\0j"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FormDownStrut[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    2,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Double, QMetaType::Int, QMetaType::Int,    6,    7,

       0        // eod
};

void FormDownStrut::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FormDownStrut *_t = static_cast<FormDownStrut *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->axis_vxhor_plus_pressed(); break;
        case 1: _t->axis_vxhor_dec_pressed(); break;
        case 2: _t->stopAxisMove(); break;
        case 3: { double _r = _t->plus_safe_check((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject FormDownStrut::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FormDownStrut.data,
      qt_meta_data_FormDownStrut,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FormDownStrut::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FormDownStrut::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FormDownStrut.stringdata0))
        return static_cast<void*>(const_cast< FormDownStrut*>(this));
    return QWidget::qt_metacast(_clname);
}

int FormDownStrut::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
