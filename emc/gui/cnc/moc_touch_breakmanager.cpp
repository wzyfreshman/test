/****************************************************************************
** Meta object code from reading C++ file 'touch_breakmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/touch_breakmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'touch_breakmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_touch_BreakManager_t {
    QByteArrayData data[8];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_touch_BreakManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_touch_BreakManager_t qt_meta_stringdata_touch_BreakManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "touch_BreakManager"
QT_MOC_LITERAL(1, 19, 7), // "showMsg"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "updateBreakInfo"
QT_MOC_LITERAL(4, 44, 4), // "path"
QT_MOC_LITERAL(5, 49, 15), // "recordBreakInfo"
QT_MOC_LITERAL(6, 65, 16), // "on_btn_0_clicked"
QT_MOC_LITERAL(7, 82, 16) // "on_btn_1_clicked"

    },
    "touch_BreakManager\0showMsg\0\0updateBreakInfo\0"
    "path\0recordBreakInfo\0on_btn_0_clicked\0"
    "on_btn_1_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_touch_BreakManager[] = {

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
       3,    1,   44,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void touch_BreakManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        touch_BreakManager *_t = static_cast<touch_BreakManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->updateBreakInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->recordBreakInfo(); break;
        case 3: _t->on_btn_0_clicked(); break;
        case 4: _t->on_btn_1_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (touch_BreakManager::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&touch_BreakManager::showMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject touch_BreakManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_touch_BreakManager.data,
      qt_meta_data_touch_BreakManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *touch_BreakManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *touch_BreakManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_touch_BreakManager.stringdata0))
        return static_cast<void*>(const_cast< touch_BreakManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int touch_BreakManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void touch_BreakManager::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
