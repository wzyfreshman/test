/****************************************************************************
** Meta object code from reading C++ file 'axispar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "newui/axispar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'axispar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AxisPar_t {
    QByteArrayData data[8];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AxisPar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AxisPar_t qt_meta_stringdata_AxisPar = {
    {
QT_MOC_LITERAL(0, 0, 7), // "AxisPar"
QT_MOC_LITERAL(1, 8, 7), // "sendMsg"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 12), // "set_maxlimit"
QT_MOC_LITERAL(4, 30, 1), // "i"
QT_MOC_LITERAL(5, 32, 12), // "set_minlimit"
QT_MOC_LITERAL(6, 45, 10), // "set_jogacc"
QT_MOC_LITERAL(7, 56, 20) // "on_btn_close_clicked"

    },
    "AxisPar\0sendMsg\0\0set_maxlimit\0i\0"
    "set_minlimit\0set_jogacc\0on_btn_close_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AxisPar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   44,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       6,    1,   50,    2, 0x08 /* Private */,
       7,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

       0        // eod
};

void AxisPar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AxisPar *_t = static_cast<AxisPar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->set_maxlimit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->set_minlimit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->set_jogacc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_btn_close_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AxisPar::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AxisPar::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AxisPar::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AxisPar.data,
      qt_meta_data_AxisPar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AxisPar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AxisPar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AxisPar.stringdata0))
        return static_cast<void*>(const_cast< AxisPar*>(this));
    return QDialog::qt_metacast(_clname);
}

int AxisPar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AxisPar::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
