/****************************************************************************
** Meta object code from reading C++ file 'inverterpar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/inverterpar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inverterpar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_inverterPar_t {
    QByteArrayData data[20];
    char stringdata0[382];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_inverterPar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_inverterPar_t qt_meta_stringdata_inverterPar = {
    {
QT_MOC_LITERAL(0, 0, 11), // "inverterPar"
QT_MOC_LITERAL(1, 12, 7), // "sendMsg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 17), // "send_payoff_speed"
QT_MOC_LITERAL(4, 39, 23), // "send_deceleration_speed"
QT_MOC_LITERAL(5, 63, 22), // "send_deceleration_time"
QT_MOC_LITERAL(6, 86, 18), // "send_payoff_speed2"
QT_MOC_LITERAL(7, 105, 24), // "send_deceleration_speed2"
QT_MOC_LITERAL(8, 130, 23), // "send_deceleration_time2"
QT_MOC_LITERAL(9, 154, 18), // "send_payoff_speed3"
QT_MOC_LITERAL(10, 173, 24), // "send_deceleration_speed3"
QT_MOC_LITERAL(11, 198, 23), // "send_deceleration_time3"
QT_MOC_LITERAL(12, 222, 18), // "send_payoff_speed4"
QT_MOC_LITERAL(13, 241, 24), // "send_deceleration_speed4"
QT_MOC_LITERAL(14, 266, 23), // "send_deceleration_time4"
QT_MOC_LITERAL(15, 290, 10), // "sendmodbus"
QT_MOC_LITERAL(16, 301, 21), // "send_payoff_speed_all"
QT_MOC_LITERAL(17, 323, 3), // "sta"
QT_MOC_LITERAL(18, 327, 27), // "send_deceleration_speed_all"
QT_MOC_LITERAL(19, 355, 26) // "send_deceleration_time_all"

    },
    "inverterPar\0sendMsg\0\0send_payoff_speed\0"
    "send_deceleration_speed\0send_deceleration_time\0"
    "send_payoff_speed2\0send_deceleration_speed2\0"
    "send_deceleration_time2\0send_payoff_speed3\0"
    "send_deceleration_speed3\0"
    "send_deceleration_time3\0send_payoff_speed4\0"
    "send_deceleration_speed4\0"
    "send_deceleration_time4\0sendmodbus\0"
    "send_payoff_speed_all\0sta\0"
    "send_deceleration_speed_all\0"
    "send_deceleration_time_all"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_inverterPar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  104,    2, 0x08 /* Private */,
       4,    0,  105,    2, 0x08 /* Private */,
       5,    0,  106,    2, 0x08 /* Private */,
       6,    0,  107,    2, 0x08 /* Private */,
       7,    0,  108,    2, 0x08 /* Private */,
       8,    0,  109,    2, 0x08 /* Private */,
       9,    0,  110,    2, 0x08 /* Private */,
      10,    0,  111,    2, 0x08 /* Private */,
      11,    0,  112,    2, 0x08 /* Private */,
      12,    0,  113,    2, 0x08 /* Private */,
      13,    0,  114,    2, 0x08 /* Private */,
      14,    0,  115,    2, 0x08 /* Private */,
      15,    0,  116,    2, 0x08 /* Private */,
      16,    1,  117,    2, 0x08 /* Private */,
      18,    1,  120,    2, 0x08 /* Private */,
      19,    1,  123,    2, 0x08 /* Private */,

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
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,

       0        // eod
};

void inverterPar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        inverterPar *_t = static_cast<inverterPar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->send_payoff_speed(); break;
        case 2: _t->send_deceleration_speed(); break;
        case 3: _t->send_deceleration_time(); break;
        case 4: _t->send_payoff_speed2(); break;
        case 5: _t->send_deceleration_speed2(); break;
        case 6: _t->send_deceleration_time2(); break;
        case 7: _t->send_payoff_speed3(); break;
        case 8: _t->send_deceleration_speed3(); break;
        case 9: _t->send_deceleration_time3(); break;
        case 10: _t->send_payoff_speed4(); break;
        case 11: _t->send_deceleration_speed4(); break;
        case 12: _t->send_deceleration_time4(); break;
        case 13: _t->sendmodbus(); break;
        case 14: _t->send_payoff_speed_all((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->send_deceleration_speed_all((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->send_deceleration_time_all((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (inverterPar::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&inverterPar::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject inverterPar::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_inverterPar.data,
      qt_meta_data_inverterPar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *inverterPar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *inverterPar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_inverterPar.stringdata0))
        return static_cast<void*>(const_cast< inverterPar*>(this));
    return QDialog::qt_metacast(_clname);
}

int inverterPar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void inverterPar::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
