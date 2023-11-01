/****************************************************************************
** Meta object code from reading C++ file 'touch_io.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/touch_io.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'touch_io.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Touch_io_t {
    QByteArrayData data[9];
    char stringdata0[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Touch_io_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Touch_io_t qt_meta_stringdata_Touch_io = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Touch_io"
QT_MOC_LITERAL(1, 9, 7), // "showMsg"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 9), // "setIOfile"
QT_MOC_LITERAL(4, 28, 8), // "filename"
QT_MOC_LITERAL(5, 37, 9), // "io_update"
QT_MOC_LITERAL(6, 47, 14), // "init_io_status"
QT_MOC_LITERAL(7, 62, 7), // "needpor"
QT_MOC_LITERAL(8, 70, 9) // "handleAlm"

    },
    "Touch_io\0showMsg\0\0setIOfile\0filename\0"
    "io_update\0init_io_status\0needpor\0"
    "handleAlm"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Touch_io[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   49,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    1,   53,    2, 0x0a /* Public */,
       6,    0,   56,    2, 0x2a /* Public | MethodCloned */,
       8,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Touch_io::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Touch_io *_t = static_cast<Touch_io *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->setIOfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->io_update(); break;
        case 3: _t->init_io_status((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->init_io_status(); break;
        case 5: _t->handleAlm(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Touch_io::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Touch_io::showMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Touch_io::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Touch_io.data,
      qt_meta_data_Touch_io,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Touch_io::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Touch_io::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Touch_io.stringdata0))
        return static_cast<void*>(const_cast< Touch_io*>(this));
    return QWidget::qt_metacast(_clname);
}

int Touch_io::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Touch_io::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
