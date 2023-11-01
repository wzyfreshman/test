/****************************************************************************
** Meta object code from reading C++ file 'formsetclamp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/formsetclamp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formsetclamp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_formSetClamp_t {
    QByteArrayData data[9];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_formSetClamp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_formSetClamp_t qt_meta_stringdata_formSetClamp = {
    {
QT_MOC_LITERAL(0, 0, 12), // "formSetClamp"
QT_MOC_LITERAL(1, 13, 17), // "on_Cancel_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 15), // "on_Scan_clicked"
QT_MOC_LITERAL(4, 48, 11), // "changeValue"
QT_MOC_LITERAL(5, 60, 5), // "value"
QT_MOC_LITERAL(6, 66, 18), // "on_MoveAll_clicked"
QT_MOC_LITERAL(7, 85, 15), // "on_Move_clicked"
QT_MOC_LITERAL(8, 101, 18) // "on_Diagram_clicked"

    },
    "formSetClamp\0on_Cancel_clicked\0\0"
    "on_Scan_clicked\0changeValue\0value\0"
    "on_MoveAll_clicked\0on_Move_clicked\0"
    "on_Diagram_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_formSetClamp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void formSetClamp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        formSetClamp *_t = static_cast<formSetClamp *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_Cancel_clicked(); break;
        case 1: _t->on_Scan_clicked(); break;
        case 2: _t->changeValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_MoveAll_clicked(); break;
        case 4: _t->on_Move_clicked(); break;
        case 5: _t->on_Diagram_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject formSetClamp::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_formSetClamp.data,
      qt_meta_data_formSetClamp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *formSetClamp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *formSetClamp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_formSetClamp.stringdata0))
        return static_cast<void*>(const_cast< formSetClamp*>(this));
    return QDialog::qt_metacast(_clname);
}

int formSetClamp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
