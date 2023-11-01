/****************************************************************************
** Meta object code from reading C++ file 'unit_funcio.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_funcio.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_funcio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_unit_FuncIO_t {
    QByteArrayData data[17];
    char stringdata0[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_FuncIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_FuncIO_t qt_meta_stringdata_unit_FuncIO = {
    {
QT_MOC_LITERAL(0, 0, 11), // "unit_FuncIO"
QT_MOC_LITERAL(1, 12, 7), // "showMsg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 14), // "auto_lube_time"
QT_MOC_LITERAL(4, 36, 20), // "lube_connect_timeout"
QT_MOC_LITERAL(5, 57, 22), // "on_btn_spindle_clicked"
QT_MOC_LITERAL(6, 80, 19), // "on_btn_cool_clicked"
QT_MOC_LITERAL(7, 100, 19), // "on_btn_lube_clicked"
QT_MOC_LITERAL(8, 120, 23), // "on_btn_spindle1_clicked"
QT_MOC_LITERAL(9, 144, 22), // "on_btn_sanding_clicked"
QT_MOC_LITERAL(10, 167, 20), // "on_btn_press_clicked"
QT_MOC_LITERAL(11, 188, 22), // "on_btn_feeding_clicked"
QT_MOC_LITERAL(12, 211, 19), // "on_btn_push_clicked"
QT_MOC_LITERAL(13, 231, 20), // "on_btn_clamp_clicked"
QT_MOC_LITERAL(14, 252, 27), // "on_btn_sandCylinder_clicked"
QT_MOC_LITERAL(15, 280, 20), // "on_btn_motor_clicked"
QT_MOC_LITERAL(16, 301, 23) // "on_btn_location_clicked"

    },
    "unit_FuncIO\0showMsg\0\0auto_lube_time\0"
    "lube_connect_timeout\0on_btn_spindle_clicked\0"
    "on_btn_cool_clicked\0on_btn_lube_clicked\0"
    "on_btn_spindle1_clicked\0on_btn_sanding_clicked\0"
    "on_btn_press_clicked\0on_btn_feeding_clicked\0"
    "on_btn_push_clicked\0on_btn_clamp_clicked\0"
    "on_btn_sandCylinder_clicked\0"
    "on_btn_motor_clicked\0on_btn_location_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_FuncIO[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   94,    2, 0x0a /* Public */,
       4,    0,   95,    2, 0x0a /* Public */,
       5,    0,   96,    2, 0x08 /* Private */,
       6,    0,   97,    2, 0x08 /* Private */,
       7,    0,   98,    2, 0x08 /* Private */,
       8,    0,   99,    2, 0x08 /* Private */,
       9,    0,  100,    2, 0x08 /* Private */,
      10,    0,  101,    2, 0x08 /* Private */,
      11,    0,  102,    2, 0x08 /* Private */,
      12,    0,  103,    2, 0x08 /* Private */,
      13,    0,  104,    2, 0x08 /* Private */,
      14,    0,  105,    2, 0x08 /* Private */,
      15,    0,  106,    2, 0x08 /* Private */,
      16,    0,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void unit_FuncIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_FuncIO *_t = static_cast<unit_FuncIO *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->auto_lube_time(); break;
        case 2: _t->lube_connect_timeout(); break;
        case 3: _t->on_btn_spindle_clicked(); break;
        case 4: _t->on_btn_cool_clicked(); break;
        case 5: _t->on_btn_lube_clicked(); break;
        case 6: _t->on_btn_spindle1_clicked(); break;
        case 7: _t->on_btn_sanding_clicked(); break;
        case 8: _t->on_btn_press_clicked(); break;
        case 9: _t->on_btn_feeding_clicked(); break;
        case 10: _t->on_btn_push_clicked(); break;
        case 11: _t->on_btn_clamp_clicked(); break;
        case 12: _t->on_btn_sandCylinder_clicked(); break;
        case 13: _t->on_btn_motor_clicked(); break;
        case 14: _t->on_btn_location_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_FuncIO::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_FuncIO::showMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject unit_FuncIO::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_unit_FuncIO.data,
      qt_meta_data_unit_FuncIO,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_FuncIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_FuncIO::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_FuncIO.stringdata0))
        return static_cast<void*>(const_cast< unit_FuncIO*>(this));
    return QWidget::qt_metacast(_clname);
}

int unit_FuncIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void unit_FuncIO::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
