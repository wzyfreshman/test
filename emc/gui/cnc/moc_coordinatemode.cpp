/****************************************************************************
** Meta object code from reading C++ file 'coordinatemode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frame/coordinatemode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'coordinatemode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CoordinateMode_t {
    QByteArrayData data[8];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CoordinateMode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CoordinateMode_t qt_meta_stringdata_CoordinateMode = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CoordinateMode"
QT_MOC_LITERAL(1, 15, 14), // "tool_signalch1"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "tool_signalch2"
QT_MOC_LITERAL(4, 46, 10), // "sendElectr"
QT_MOC_LITERAL(5, 57, 16), // "insert_info_line"
QT_MOC_LITERAL(6, 74, 5), // "index"
QT_MOC_LITERAL(7, 80, 3) // "msg"

    },
    "CoordinateMode\0tool_signalch1\0\0"
    "tool_signalch2\0sendElectr\0insert_info_line\0"
    "index\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CoordinateMode[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    1,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    6,    7,

       0        // eod
};

void CoordinateMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CoordinateMode *_t = static_cast<CoordinateMode *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tool_signalch1(); break;
        case 1: _t->tool_signalch2(); break;
        case 2: _t->sendElectr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->insert_info_line((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CoordinateMode::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoordinateMode::tool_signalch1)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CoordinateMode::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoordinateMode::tool_signalch2)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CoordinateMode::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoordinateMode::sendElectr)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject CoordinateMode::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CoordinateMode.data,
      qt_meta_data_CoordinateMode,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CoordinateMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CoordinateMode::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CoordinateMode.stringdata0))
        return static_cast<void*>(const_cast< CoordinateMode*>(this));
    return QWidget::qt_metacast(_clname);
}

int CoordinateMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CoordinateMode::tool_signalch1()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CoordinateMode::tool_signalch2()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CoordinateMode::sendElectr(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
