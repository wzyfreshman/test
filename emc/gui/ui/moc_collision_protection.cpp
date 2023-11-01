/****************************************************************************
** Meta object code from reading C++ file 'collision_protection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "collision_protection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'collision_protection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Collision_protection_t {
    QByteArrayData data[15];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Collision_protection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Collision_protection_t qt_meta_stringdata_Collision_protection = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Collision_protection"
QT_MOC_LITERAL(1, 21, 8), // "show_msg"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "set_y_side"
QT_MOC_LITERAL(4, 42, 15), // "set_y_frontside"
QT_MOC_LITERAL(5, 58, 14), // "set_y_rearside"
QT_MOC_LITERAL(6, 73, 10), // "set_z_side"
QT_MOC_LITERAL(7, 84, 15), // "set_z_frontside"
QT_MOC_LITERAL(8, 100, 14), // "set_z_rearside"
QT_MOC_LITERAL(9, 115, 10), // "set_a_side"
QT_MOC_LITERAL(10, 126, 15), // "set_a_frontside"
QT_MOC_LITERAL(11, 142, 14), // "set_a_rearside"
QT_MOC_LITERAL(12, 157, 10), // "set_b_side"
QT_MOC_LITERAL(13, 168, 15), // "set_b_frontside"
QT_MOC_LITERAL(14, 184, 14) // "set_b_rearside"

    },
    "Collision_protection\0show_msg\0\0"
    "set_y_side\0set_y_frontside\0set_y_rearside\0"
    "set_z_side\0set_z_frontside\0set_z_rearside\0"
    "set_a_side\0set_a_frontside\0set_a_rearside\0"
    "set_b_side\0set_b_frontside\0set_b_rearside"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Collision_protection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   84,    2, 0x08 /* Private */,
       4,    0,   85,    2, 0x08 /* Private */,
       5,    0,   86,    2, 0x08 /* Private */,
       6,    0,   87,    2, 0x08 /* Private */,
       7,    0,   88,    2, 0x08 /* Private */,
       8,    0,   89,    2, 0x08 /* Private */,
       9,    0,   90,    2, 0x08 /* Private */,
      10,    0,   91,    2, 0x08 /* Private */,
      11,    0,   92,    2, 0x08 /* Private */,
      12,    0,   93,    2, 0x08 /* Private */,
      13,    0,   94,    2, 0x08 /* Private */,
      14,    0,   95,    2, 0x08 /* Private */,

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

       0        // eod
};

void Collision_protection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Collision_protection *_t = static_cast<Collision_protection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->set_y_side(); break;
        case 2: _t->set_y_frontside(); break;
        case 3: _t->set_y_rearside(); break;
        case 4: _t->set_z_side(); break;
        case 5: _t->set_z_frontside(); break;
        case 6: _t->set_z_rearside(); break;
        case 7: _t->set_a_side(); break;
        case 8: _t->set_a_frontside(); break;
        case 9: _t->set_a_rearside(); break;
        case 10: _t->set_b_side(); break;
        case 11: _t->set_b_frontside(); break;
        case 12: _t->set_b_rearside(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Collision_protection::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Collision_protection::show_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Collision_protection::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Collision_protection.data,
      qt_meta_data_Collision_protection,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Collision_protection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Collision_protection::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Collision_protection.stringdata0))
        return static_cast<void*>(const_cast< Collision_protection*>(this));
    return QDialog::qt_metacast(_clname);
}

int Collision_protection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void Collision_protection::show_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
