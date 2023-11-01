/****************************************************************************
** Meta object code from reading C++ file 'formswingsetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/formswingsetting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formswingsetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FormSwingSetting_t {
    QByteArrayData data[12];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FormSwingSetting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FormSwingSetting_t qt_meta_stringdata_FormSwingSetting = {
    {
QT_MOC_LITERAL(0, 0, 16), // "FormSwingSetting"
QT_MOC_LITERAL(1, 17, 18), // "ValueChangeForDelZ"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 3), // "val"
QT_MOC_LITERAL(4, 41, 21), // "ValueChangeForDelZArm"
QT_MOC_LITERAL(5, 63, 19), // "on_RecordY1_clicked"
QT_MOC_LITERAL(6, 83, 19), // "on_RecordY2_clicked"
QT_MOC_LITERAL(7, 103, 19), // "on_RecordZ1_clicked"
QT_MOC_LITERAL(8, 123, 19), // "on_RecordZ2_clicked"
QT_MOC_LITERAL(9, 143, 24), // "on_CalculateDelZ_clicked"
QT_MOC_LITERAL(10, 168, 24), // "on_CalculateDelY_clicked"
QT_MOC_LITERAL(11, 193, 21) // "on_btn_return_clicked"

    },
    "FormSwingSetting\0ValueChangeForDelZ\0"
    "\0val\0ValueChangeForDelZArm\0"
    "on_RecordY1_clicked\0on_RecordY2_clicked\0"
    "on_RecordZ1_clicked\0on_RecordZ2_clicked\0"
    "on_CalculateDelZ_clicked\0"
    "on_CalculateDelY_clicked\0on_btn_return_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FormSwingSetting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08 /* Private */,
       4,    1,   62,    2, 0x08 /* Private */,
       5,    0,   65,    2, 0x08 /* Private */,
       6,    0,   66,    2, 0x08 /* Private */,
       7,    0,   67,    2, 0x08 /* Private */,
       8,    0,   68,    2, 0x08 /* Private */,
       9,    0,   69,    2, 0x08 /* Private */,
      10,    0,   70,    2, 0x08 /* Private */,
      11,    0,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FormSwingSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FormSwingSetting *_t = static_cast<FormSwingSetting *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ValueChangeForDelZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->ValueChangeForDelZArm((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->on_RecordY1_clicked(); break;
        case 3: _t->on_RecordY2_clicked(); break;
        case 4: _t->on_RecordZ1_clicked(); break;
        case 5: _t->on_RecordZ2_clicked(); break;
        case 6: _t->on_CalculateDelZ_clicked(); break;
        case 7: _t->on_CalculateDelY_clicked(); break;
        case 8: _t->on_btn_return_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject FormSwingSetting::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FormSwingSetting.data,
      qt_meta_data_FormSwingSetting,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FormSwingSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FormSwingSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FormSwingSetting.stringdata0))
        return static_cast<void*>(const_cast< FormSwingSetting*>(this));
    return QDialog::qt_metacast(_clname);
}

int FormSwingSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
