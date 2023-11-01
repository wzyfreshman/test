/****************************************************************************
** Meta object code from reading C++ file 'pz_posresult.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/pz_posresult.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pz_posresult.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PZ_PositionManagerSIG_t {
    QByteArrayData data[3];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PZ_PositionManagerSIG_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PZ_PositionManagerSIG_t qt_meta_stringdata_PZ_PositionManagerSIG = {
    {
QT_MOC_LITERAL(0, 0, 21), // "PZ_PositionManagerSIG"
QT_MOC_LITERAL(1, 22, 7), // "sendMsg"
QT_MOC_LITERAL(2, 30, 0) // ""

    },
    "PZ_PositionManagerSIG\0sendMsg\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PZ_PositionManagerSIG[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

       0        // eod
};

void PZ_PositionManagerSIG::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PZ_PositionManagerSIG *_t = static_cast<PZ_PositionManagerSIG *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PZ_PositionManagerSIG::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PZ_PositionManagerSIG::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject PZ_PositionManagerSIG::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PZ_PositionManagerSIG.data,
      qt_meta_data_PZ_PositionManagerSIG,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PZ_PositionManagerSIG::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PZ_PositionManagerSIG::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PZ_PositionManagerSIG.stringdata0))
        return static_cast<void*>(const_cast< PZ_PositionManagerSIG*>(this));
    return QObject::qt_metacast(_clname);
}

int PZ_PositionManagerSIG::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void PZ_PositionManagerSIG::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
