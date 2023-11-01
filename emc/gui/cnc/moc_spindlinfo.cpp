/****************************************************************************
** Meta object code from reading C++ file 'spindlinfo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/spindlinfo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spindlinfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SpindlInfo_t {
    QByteArrayData data[14];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpindlInfo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpindlInfo_t qt_meta_stringdata_SpindlInfo = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SpindlInfo"
QT_MOC_LITERAL(1, 11, 7), // "showMsg"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 11), // "tool_signal"
QT_MOC_LITERAL(4, 32, 14), // "setElectricity"
QT_MOC_LITERAL(5, 47, 3), // "val"
QT_MOC_LITERAL(6, 51, 13), // "set_feed_rate"
QT_MOC_LITERAL(7, 65, 16), // "set_spindle_feed"
QT_MOC_LITERAL(8, 82, 17), // "set_spindle_feed2"
QT_MOC_LITERAL(9, 100, 17), // "set_feed_override"
QT_MOC_LITERAL(10, 118, 18), // "set_feed_overrides"
QT_MOC_LITERAL(11, 137, 20), // "set_spindle_override"
QT_MOC_LITERAL(12, 158, 20), // "set_showpolishingval"
QT_MOC_LITERAL(13, 179, 15) // "clear_workpiece"

    },
    "SpindlInfo\0showMsg\0\0tool_signal\0"
    "setElectricity\0val\0set_feed_rate\0"
    "set_spindle_feed\0set_spindle_feed2\0"
    "set_feed_override\0set_feed_overrides\0"
    "set_spindle_override\0set_showpolishingval\0"
    "clear_workpiece"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpindlInfo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,
       3,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   75,    2, 0x0a /* Public */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    0,   81,    2, 0x08 /* Private */,
      10,    0,   82,    2, 0x08 /* Private */,
      11,    0,   83,    2, 0x08 /* Private */,
      12,    0,   84,    2, 0x08 /* Private */,
      13,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SpindlInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SpindlInfo *_t = static_cast<SpindlInfo *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->tool_signal(); break;
        case 2: _t->setElectricity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->set_feed_rate(); break;
        case 4: _t->set_spindle_feed(); break;
        case 5: _t->set_spindle_feed2(); break;
        case 6: _t->set_feed_override(); break;
        case 7: _t->set_feed_overrides(); break;
        case 8: _t->set_spindle_override(); break;
        case 9: _t->set_showpolishingval(); break;
        case 10: _t->clear_workpiece(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SpindlInfo::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpindlInfo::showMsg)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SpindlInfo::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpindlInfo::tool_signal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SpindlInfo::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SpindlInfo.data,
      qt_meta_data_SpindlInfo,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SpindlInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpindlInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SpindlInfo.stringdata0))
        return static_cast<void*>(const_cast< SpindlInfo*>(this));
    return QWidget::qt_metacast(_clname);
}

int SpindlInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SpindlInfo::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SpindlInfo::tool_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
