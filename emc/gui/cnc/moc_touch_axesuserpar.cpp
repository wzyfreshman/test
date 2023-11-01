/****************************************************************************
** Meta object code from reading C++ file 'touch_axesuserpar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/touch_axesuserpar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'touch_axesuserpar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_touch_AxesUserpar_t {
    QByteArrayData data[14];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_touch_AxesUserpar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_touch_AxesUserpar_t qt_meta_stringdata_touch_AxesUserpar = {
    {
QT_MOC_LITERAL(0, 0, 17), // "touch_AxesUserpar"
QT_MOC_LITERAL(1, 18, 7), // "showMsg"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "initMachine"
QT_MOC_LITERAL(4, 39, 8), // "updateui"
QT_MOC_LITERAL(5, 48, 25), // "sp_pendulum1_valueChanged"
QT_MOC_LITERAL(6, 74, 4), // "arg1"
QT_MOC_LITERAL(7, 79, 25), // "sp_pendulum2_valueChanged"
QT_MOC_LITERAL(8, 105, 26), // "sp_turntable1_valueChanged"
QT_MOC_LITERAL(9, 132, 26), // "sp_turntable2_valueChanged"
QT_MOC_LITERAL(10, 159, 27), // "sp_pendulum1_3_valueChanged"
QT_MOC_LITERAL(11, 187, 27), // "sp_pendulum2_3_valueChanged"
QT_MOC_LITERAL(12, 215, 28), // "sp_turntable1_3_valueChanged"
QT_MOC_LITERAL(13, 244, 28) // "sp_turntable2_3_valueChanged"

    },
    "touch_AxesUserpar\0showMsg\0\0initMachine\0"
    "updateui\0sp_pendulum1_valueChanged\0"
    "arg1\0sp_pendulum2_valueChanged\0"
    "sp_turntable1_valueChanged\0"
    "sp_turntable2_valueChanged\0"
    "sp_pendulum1_3_valueChanged\0"
    "sp_pendulum2_3_valueChanged\0"
    "sp_turntable1_3_valueChanged\0"
    "sp_turntable2_3_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_touch_AxesUserpar[] = {

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
       3,    0,   74,    2, 0x0a /* Public */,
       4,    0,   75,    2, 0x0a /* Public */,
       5,    1,   76,    2, 0x08 /* Private */,
       7,    1,   79,    2, 0x08 /* Private */,
       8,    1,   82,    2, 0x08 /* Private */,
       9,    1,   85,    2, 0x08 /* Private */,
      10,    1,   88,    2, 0x08 /* Private */,
      11,    1,   91,    2, 0x08 /* Private */,
      12,    1,   94,    2, 0x08 /* Private */,
      13,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,

       0        // eod
};

void touch_AxesUserpar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        touch_AxesUserpar *_t = static_cast<touch_AxesUserpar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->initMachine(); break;
        case 2: _t->updateui(); break;
        case 3: _t->sp_pendulum1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->sp_pendulum2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->sp_turntable1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->sp_turntable2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->sp_pendulum1_3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->sp_pendulum2_3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->sp_turntable1_3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->sp_turntable2_3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (touch_AxesUserpar::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&touch_AxesUserpar::showMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject touch_AxesUserpar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_touch_AxesUserpar.data,
      qt_meta_data_touch_AxesUserpar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *touch_AxesUserpar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *touch_AxesUserpar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_touch_AxesUserpar.stringdata0))
        return static_cast<void*>(const_cast< touch_AxesUserpar*>(this));
    return QWidget::qt_metacast(_clname);
}

int touch_AxesUserpar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void touch_AxesUserpar::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
