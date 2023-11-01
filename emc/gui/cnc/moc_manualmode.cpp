/****************************************************************************
** Meta object code from reading C++ file 'manualmode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frame/manualmode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'manualmode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ManualMode_t {
    QByteArrayData data[18];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ManualMode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ManualMode_t qt_meta_stringdata_ManualMode = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ManualMode"
QT_MOC_LITERAL(1, 11, 7), // "showMsg"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 11), // "setBrushdir"
QT_MOC_LITERAL(4, 32, 8), // "updateUI"
QT_MOC_LITERAL(5, 41, 12), // "updateStatus"
QT_MOC_LITERAL(6, 54, 10), // "setOverrun"
QT_MOC_LITERAL(7, 65, 7), // "setZero"
QT_MOC_LITERAL(8, 73, 13), // "startAxisMove"
QT_MOC_LITERAL(9, 87, 4), // "axis"
QT_MOC_LITERAL(10, 92, 3), // "dir"
QT_MOC_LITERAL(11, 96, 12), // "stopAxisMove"
QT_MOC_LITERAL(12, 109, 21), // "ValueChangeForG0Speed"
QT_MOC_LITERAL(13, 131, 3), // "val"
QT_MOC_LITERAL(14, 135, 24), // "ValueChangeForMslowSpeed"
QT_MOC_LITERAL(15, 160, 24), // "ValueChangeForMhighSpeed"
QT_MOC_LITERAL(16, 185, 24), // "ValueChangeForClampDelay"
QT_MOC_LITERAL(17, 210, 24) // "ValueChangeForPressAvoid"

    },
    "ManualMode\0showMsg\0\0setBrushdir\0"
    "updateUI\0updateStatus\0setOverrun\0"
    "setZero\0startAxisMove\0axis\0dir\0"
    "stopAxisMove\0ValueChangeForG0Speed\0"
    "val\0ValueChangeForMslowSpeed\0"
    "ValueChangeForMhighSpeed\0"
    "ValueChangeForClampDelay\0"
    "ValueChangeForPressAvoid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ManualMode[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       3,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   87,    2, 0x0a /* Public */,
       5,    0,   88,    2, 0x0a /* Public */,
       6,    0,   89,    2, 0x0a /* Public */,
       7,    0,   90,    2, 0x0a /* Public */,
       8,    2,   91,    2, 0x0a /* Public */,
      11,    2,   96,    2, 0x0a /* Public */,
      12,    1,  101,    2, 0x08 /* Private */,
      14,    1,  104,    2, 0x08 /* Private */,
      15,    1,  107,    2, 0x08 /* Private */,
      16,    1,  110,    2, 0x08 /* Private */,
      17,    1,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, QMetaType::Double,   13,

       0        // eod
};

void ManualMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ManualMode *_t = static_cast<ManualMode *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->setBrushdir((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateUI(); break;
        case 3: _t->updateStatus(); break;
        case 4: _t->setOverrun(); break;
        case 5: _t->setZero(); break;
        case 6: _t->startAxisMove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->stopAxisMove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->ValueChangeForG0Speed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->ValueChangeForMslowSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->ValueChangeForMhighSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->ValueChangeForClampDelay((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->ValueChangeForPressAvoid((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ManualMode::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ManualMode::showMsg)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ManualMode::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ManualMode::setBrushdir)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ManualMode::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ManualMode.data,
      qt_meta_data_ManualMode,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ManualMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ManualMode::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ManualMode.stringdata0))
        return static_cast<void*>(const_cast< ManualMode*>(this));
    return QWidget::qt_metacast(_clname);
}

int ManualMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ManualMode::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ManualMode::setBrushdir(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
