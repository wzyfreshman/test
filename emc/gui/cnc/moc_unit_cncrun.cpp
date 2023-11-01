/****************************************************************************
** Meta object code from reading C++ file 'unit_cncrun.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_cncrun.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_cncrun.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_unit_cncrun_t {
    QByteArrayData data[6];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_cncrun_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_cncrun_t qt_meta_stringdata_unit_cncrun = {
    {
QT_MOC_LITERAL(0, 0, 11), // "unit_cncrun"
QT_MOC_LITERAL(1, 12, 8), // "updateUI"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 10), // "runPressed"
QT_MOC_LITERAL(4, 33, 12), // "pausePressed"
QT_MOC_LITERAL(5, 46, 12) // "abortPressed"

    },
    "unit_cncrun\0updateUI\0\0runPressed\0"
    "pausePressed\0abortPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_cncrun[] = {

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
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void unit_cncrun::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_cncrun *_t = static_cast<unit_cncrun *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateUI(); break;
        case 1: _t->runPressed(); break;
        case 2: _t->pausePressed(); break;
        case 3: _t->abortPressed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject unit_cncrun::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_unit_cncrun.data,
      qt_meta_data_unit_cncrun,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_cncrun::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_cncrun::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_cncrun.stringdata0))
        return static_cast<void*>(const_cast< unit_cncrun*>(this));
    return QWidget::qt_metacast(_clname);
}

int unit_cncrun::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
