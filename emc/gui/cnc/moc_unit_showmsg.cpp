/****************************************************************************
** Meta object code from reading C++ file 'unit_showmsg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_showmsg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_showmsg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_unit_showMsg_t {
    QByteArrayData data[9];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_showMsg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_showMsg_t qt_meta_stringdata_unit_showMsg = {
    {
QT_MOC_LITERAL(0, 0, 12), // "unit_showMsg"
QT_MOC_LITERAL(1, 13, 13), // "info_to_table"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "showMsg"
QT_MOC_LITERAL(4, 36, 5), // "_type"
QT_MOC_LITERAL(5, 42, 4), // "_msg"
QT_MOC_LITERAL(6, 47, 6), // "clrMsg"
QT_MOC_LITERAL(7, 54, 12), // "setTimeWidth"
QT_MOC_LITERAL(8, 67, 5) // "width"

    },
    "unit_showMsg\0info_to_table\0\0showMsg\0"
    "_type\0_msg\0clrMsg\0setTimeWidth\0width"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_showMsg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   39,    2, 0x0a /* Public */,
       6,    0,   44,    2, 0x0a /* Public */,
       7,    1,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void unit_showMsg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_showMsg *_t = static_cast<unit_showMsg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->info_to_table((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->clrMsg(); break;
        case 3: _t->setTimeWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_showMsg::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_showMsg::info_to_table)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject unit_showMsg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_unit_showMsg.data,
      qt_meta_data_unit_showMsg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_showMsg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_showMsg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_showMsg.stringdata0))
        return static_cast<void*>(const_cast< unit_showMsg*>(this));
    return QWidget::qt_metacast(_clname);
}

int unit_showMsg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void unit_showMsg::info_to_table(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
