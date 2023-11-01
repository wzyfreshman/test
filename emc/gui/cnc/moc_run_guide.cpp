/****************************************************************************
** Meta object code from reading C++ file 'run_guide.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MZB/run_guide.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'run_guide.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_run_guide_t {
    QByteArrayData data[12];
    char stringdata0[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_run_guide_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_run_guide_t qt_meta_stringdata_run_guide = {
    {
QT_MOC_LITERAL(0, 0, 9), // "run_guide"
QT_MOC_LITERAL(1, 10, 7), // "sendMsg"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 10), // "updatetips"
QT_MOC_LITERAL(4, 30, 19), // "on_btn_move_clicked"
QT_MOC_LITERAL(5, 50, 20), // "on_btn_close_clicked"
QT_MOC_LITERAL(6, 71, 19), // "on_btn_stop_clicked"
QT_MOC_LITERAL(7, 91, 23), // "on_btn_location_clicked"
QT_MOC_LITERAL(8, 115, 20), // "on_btn_drill_clicked"
QT_MOC_LITERAL(9, 136, 21), // "on_btn_moveab_clicked"
QT_MOC_LITERAL(10, 158, 24), // "on_btn_insertion_clicked"
QT_MOC_LITERAL(11, 183, 21) // "on_btn_Remove_clicked"

    },
    "run_guide\0sendMsg\0\0updatetips\0"
    "on_btn_move_clicked\0on_btn_close_clicked\0"
    "on_btn_stop_clicked\0on_btn_location_clicked\0"
    "on_btn_drill_clicked\0on_btn_moveab_clicked\0"
    "on_btn_insertion_clicked\0on_btn_Remove_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_run_guide[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   69,    2, 0x0a /* Public */,
       4,    0,   70,    2, 0x08 /* Private */,
       5,    0,   71,    2, 0x08 /* Private */,
       6,    0,   72,    2, 0x08 /* Private */,
       7,    0,   73,    2, 0x08 /* Private */,
       8,    0,   74,    2, 0x08 /* Private */,
       9,    0,   75,    2, 0x08 /* Private */,
      10,    0,   76,    2, 0x08 /* Private */,
      11,    0,   77,    2, 0x08 /* Private */,

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

       0        // eod
};

void run_guide::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        run_guide *_t = static_cast<run_guide *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->updatetips(); break;
        case 2: _t->on_btn_move_clicked(); break;
        case 3: _t->on_btn_close_clicked(); break;
        case 4: _t->on_btn_stop_clicked(); break;
        case 5: _t->on_btn_location_clicked(); break;
        case 6: _t->on_btn_drill_clicked(); break;
        case 7: _t->on_btn_moveab_clicked(); break;
        case 8: _t->on_btn_insertion_clicked(); break;
        case 9: _t->on_btn_Remove_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (run_guide::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&run_guide::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject run_guide::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_run_guide.data,
      qt_meta_data_run_guide,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *run_guide::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *run_guide::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_run_guide.stringdata0))
        return static_cast<void*>(const_cast< run_guide*>(this));
    return QDialog::qt_metacast(_clname);
}

int run_guide::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void run_guide::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
