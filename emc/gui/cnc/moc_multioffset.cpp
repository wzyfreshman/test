/****************************************************************************
** Meta object code from reading C++ file 'multioffset.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/multioffset.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multioffset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GroupPos_t {
    QByteArrayData data[3];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GroupPos_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GroupPos_t qt_meta_stringdata_GroupPos = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GroupPos"
QT_MOC_LITERAL(1, 9, 9), // "changePos"
QT_MOC_LITERAL(2, 19, 0) // ""

    },
    "GroupPos\0changePos\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GroupPos[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void GroupPos::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GroupPos *_t = static_cast<GroupPos *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changePos((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject GroupPos::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GroupPos.data,
      qt_meta_data_GroupPos,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GroupPos::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GroupPos::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GroupPos.stringdata0))
        return static_cast<void*>(const_cast< GroupPos*>(this));
    return QWidget::qt_metacast(_clname);
}

int GroupPos::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_unit_multioffset_t {
    QByteArrayData data[19];
    char stringdata0[376];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_multioffset_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_multioffset_t qt_meta_stringdata_unit_multioffset = {
    {
QT_MOC_LITERAL(0, 0, 16), // "unit_multioffset"
QT_MOC_LITERAL(1, 17, 7), // "sendMsg"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 22), // "on_btn_dec_001_clicked"
QT_MOC_LITERAL(4, 49, 21), // "on_btn_dec_01_clicked"
QT_MOC_LITERAL(5, 71, 20), // "on_btn_dec_1_clicked"
QT_MOC_LITERAL(6, 92, 24), // "on_btn_raise_001_clicked"
QT_MOC_LITERAL(7, 117, 23), // "on_btn_raise_01_clicked"
QT_MOC_LITERAL(8, 141, 22), // "on_btn_raise_1_clicked"
QT_MOC_LITERAL(9, 164, 18), // "on_set_g54_clicked"
QT_MOC_LITERAL(10, 183, 18), // "on_set_g55_clicked"
QT_MOC_LITERAL(11, 202, 18), // "on_set_g56_clicked"
QT_MOC_LITERAL(12, 221, 18), // "on_set_g57_clicked"
QT_MOC_LITERAL(13, 240, 18), // "on_set_g58_clicked"
QT_MOC_LITERAL(14, 259, 18), // "on_set_g59_clicked"
QT_MOC_LITERAL(15, 278, 23), // "on_btn_record_x_clicked"
QT_MOC_LITERAL(16, 302, 23), // "on_btn_record_y_clicked"
QT_MOC_LITERAL(17, 326, 24), // "on_btn_record_x2_clicked"
QT_MOC_LITERAL(18, 351, 24) // "on_btn_record_y2_clicked"

    },
    "unit_multioffset\0sendMsg\0\0"
    "on_btn_dec_001_clicked\0on_btn_dec_01_clicked\0"
    "on_btn_dec_1_clicked\0on_btn_raise_001_clicked\0"
    "on_btn_raise_01_clicked\0on_btn_raise_1_clicked\0"
    "on_set_g54_clicked\0on_set_g55_clicked\0"
    "on_set_g56_clicked\0on_set_g57_clicked\0"
    "on_set_g58_clicked\0on_set_g59_clicked\0"
    "on_btn_record_x_clicked\0on_btn_record_y_clicked\0"
    "on_btn_record_x2_clicked\0"
    "on_btn_record_y2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_multioffset[] = {

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
      16,    0,  117,    2, 0x08 /* Private */,
      17,    0,  118,    2, 0x08 /* Private */,
      18,    0,  119,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void unit_multioffset::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_multioffset *_t = static_cast<unit_multioffset *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->on_btn_dec_001_clicked(); break;
        case 2: _t->on_btn_dec_01_clicked(); break;
        case 3: _t->on_btn_dec_1_clicked(); break;
        case 4: _t->on_btn_raise_001_clicked(); break;
        case 5: _t->on_btn_raise_01_clicked(); break;
        case 6: _t->on_btn_raise_1_clicked(); break;
        case 7: _t->on_set_g54_clicked(); break;
        case 8: _t->on_set_g55_clicked(); break;
        case 9: _t->on_set_g56_clicked(); break;
        case 10: _t->on_set_g57_clicked(); break;
        case 11: _t->on_set_g58_clicked(); break;
        case 12: _t->on_set_g59_clicked(); break;
        case 13: _t->on_btn_record_x_clicked(); break;
        case 14: _t->on_btn_record_y_clicked(); break;
        case 15: _t->on_btn_record_x2_clicked(); break;
        case 16: _t->on_btn_record_y2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_multioffset::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_multioffset::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject unit_multioffset::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_unit_multioffset.data,
      qt_meta_data_unit_multioffset,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_multioffset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_multioffset::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_multioffset.stringdata0))
        return static_cast<void*>(const_cast< unit_multioffset*>(this));
    return QDialog::qt_metacast(_clname);
}

int unit_multioffset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void unit_multioffset::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_GetValue_t {
    QByteArrayData data[4];
    char stringdata0[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GetValue_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GetValue_t qt_meta_stringdata_GetValue = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GetValue"
QT_MOC_LITERAL(1, 9, 14), // "ensureGetValue"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 6) // "getPos"

    },
    "GetValue\0ensureGetValue\0\0getPos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GetValue[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GetValue::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GetValue *_t = static_cast<GetValue *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensureGetValue(); break;
        case 1: _t->getPos(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GetValue::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GetValue.data,
      qt_meta_data_GetValue,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GetValue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GetValue::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GetValue.stringdata0))
        return static_cast<void*>(const_cast< GetValue*>(this));
    return QDialog::qt_metacast(_clname);
}

int GetValue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
