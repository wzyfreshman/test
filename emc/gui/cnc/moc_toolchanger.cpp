/****************************************************************************
** Meta object code from reading C++ file 'toolchanger.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/toolchanger.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolchanger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ToolChanger_t {
    QByteArrayData data[15];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolChanger_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolChanger_t qt_meta_stringdata_ToolChanger = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ToolChanger"
QT_MOC_LITERAL(1, 12, 8), // "send_msg"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 3), // "str"
QT_MOC_LITERAL(4, 26, 19), // "change_to_chosetool"
QT_MOC_LITERAL(5, 46, 12), // "setSpindleIO"
QT_MOC_LITERAL(6, 59, 19), // "setToolChangerCover"
QT_MOC_LITERAL(7, 79, 10), // "ToolUpdate"
QT_MOC_LITERAL(8, 90, 12), // "changetoTool"
QT_MOC_LITERAL(9, 103, 12), // "setBtnEnable"
QT_MOC_LITERAL(10, 116, 2), // "tr"
QT_MOC_LITERAL(11, 119, 17), // "on_move_c_clicked"
QT_MOC_LITERAL(12, 137, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(13, 161, 21), // "on_btn_resetC_clicked"
QT_MOC_LITERAL(14, 183, 15) // "on_blow_clicked"

    },
    "ToolChanger\0send_msg\0\0str\0change_to_chosetool\0"
    "setSpindleIO\0setToolChangerCover\0"
    "ToolUpdate\0changetoTool\0setBtnEnable\0"
    "tr\0on_move_c_clicked\0on_pushButton_2_clicked\0"
    "on_btn_resetC_clicked\0on_blow_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolChanger[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   74,    2, 0x0a /* Public */,
       5,    0,   75,    2, 0x0a /* Public */,
       6,    0,   76,    2, 0x0a /* Public */,
       7,    0,   77,    2, 0x0a /* Public */,
       8,    0,   78,    2, 0x0a /* Public */,
       9,    1,   79,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x08 /* Private */,
      12,    0,   83,    2, 0x08 /* Private */,
      13,    0,   84,    2, 0x08 /* Private */,
      14,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ToolChanger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolChanger *_t = static_cast<ToolChanger *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->change_to_chosetool(); break;
        case 2: _t->setSpindleIO(); break;
        case 3: _t->setToolChangerCover(); break;
        case 4: _t->ToolUpdate(); break;
        case 5: _t->changetoTool(); break;
        case 6: _t->setBtnEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_move_c_clicked(); break;
        case 8: _t->on_pushButton_2_clicked(); break;
        case 9: _t->on_btn_resetC_clicked(); break;
        case 10: _t->on_blow_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ToolChanger::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ToolChanger::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ToolChanger::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ToolChanger.data,
      qt_meta_data_ToolChanger,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ToolChanger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolChanger::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ToolChanger.stringdata0))
        return static_cast<void*>(const_cast< ToolChanger*>(this));
    return QWidget::qt_metacast(_clname);
}

int ToolChanger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ToolChanger::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
