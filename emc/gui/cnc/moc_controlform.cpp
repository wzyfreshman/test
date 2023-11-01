/****************************************************************************
** Meta object code from reading C++ file 'controlform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frame/controlform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_controlform_t {
    QByteArrayData data[13];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_controlform_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_controlform_t qt_meta_stringdata_controlform = {
    {
QT_MOC_LITERAL(0, 0, 11), // "controlform"
QT_MOC_LITERAL(1, 12, 7), // "showMsg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 8), // "reloadnc"
QT_MOC_LITERAL(4, 30, 19), // "slotSetSpindleSpeed"
QT_MOC_LITERAL(5, 50, 3), // "vel"
QT_MOC_LITERAL(6, 54, 8), // "setEstop"
QT_MOC_LITERAL(7, 63, 3), // "flg"
QT_MOC_LITERAL(8, 67, 9), // "sendAbort"
QT_MOC_LITERAL(9, 77, 7), // "sendRun"
QT_MOC_LITERAL(10, 85, 16), // "sendFeedOverride"
QT_MOC_LITERAL(11, 102, 4), // "rate"
QT_MOC_LITERAL(12, 107, 19) // "sendSpindleOverride"

    },
    "controlform\0showMsg\0\0reloadnc\0"
    "slotSetSpindleSpeed\0vel\0setEstop\0flg\0"
    "sendAbort\0sendRun\0sendFeedOverride\0"
    "rate\0sendSpindleOverride"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_controlform[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       3,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   62,    2, 0x0a /* Public */,
       6,    1,   65,    2, 0x0a /* Public */,
       8,    0,   68,    2, 0x0a /* Public */,
       9,    0,   69,    2, 0x0a /* Public */,
      10,    1,   70,    2, 0x0a /* Public */,
      12,    1,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void controlform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        controlform *_t = static_cast<controlform *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->reloadnc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotSetSpindleSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setEstop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->sendAbort(); break;
        case 5: _t->sendRun(); break;
        case 6: _t->sendFeedOverride((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->sendSpindleOverride((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (controlform::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&controlform::showMsg)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (controlform::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&controlform::reloadnc)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject controlform::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_controlform.data,
      qt_meta_data_controlform,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *controlform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *controlform::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_controlform.stringdata0))
        return static_cast<void*>(const_cast< controlform*>(this));
    return QWidget::qt_metacast(_clname);
}

int controlform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void controlform::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void controlform::reloadnc(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
