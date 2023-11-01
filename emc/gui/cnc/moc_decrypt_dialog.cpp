/****************************************************************************
** Meta object code from reading C++ file 'decrypt_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/decrypt_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'decrypt_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Decrypt_Dialog_t {
    QByteArrayData data[17];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Decrypt_Dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Decrypt_Dialog_t qt_meta_stringdata_Decrypt_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Decrypt_Dialog"
QT_MOC_LITERAL(1, 15, 8), // "send_msg"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "slotCode1"
QT_MOC_LITERAL(4, 35, 9), // "slotCode2"
QT_MOC_LITERAL(5, 45, 9), // "slotCode3"
QT_MOC_LITERAL(6, 55, 9), // "slotCode4"
QT_MOC_LITERAL(7, 65, 11), // "slotCode1_2"
QT_MOC_LITERAL(8, 77, 11), // "slotCode2_2"
QT_MOC_LITERAL(9, 89, 11), // "slotCode3_2"
QT_MOC_LITERAL(10, 101, 11), // "slotCode4_2"
QT_MOC_LITERAL(11, 113, 7), // "slotVer"
QT_MOC_LITERAL(12, 121, 7), // "slotSys"
QT_MOC_LITERAL(13, 129, 8), // "slotSign"
QT_MOC_LITERAL(14, 138, 6), // "update"
QT_MOC_LITERAL(15, 145, 12), // "setSysString"
QT_MOC_LITERAL(16, 158, 19) // "on_Btn_More_clicked"

    },
    "Decrypt_Dialog\0send_msg\0\0slotCode1\0"
    "slotCode2\0slotCode3\0slotCode4\0slotCode1_2\0"
    "slotCode2_2\0slotCode3_2\0slotCode4_2\0"
    "slotVer\0slotSys\0slotSign\0update\0"
    "setSysString\0on_Btn_More_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Decrypt_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   94,    2, 0x0a /* Public */,
       4,    0,   95,    2, 0x0a /* Public */,
       5,    0,   96,    2, 0x0a /* Public */,
       6,    0,   97,    2, 0x0a /* Public */,
       7,    0,   98,    2, 0x0a /* Public */,
       8,    0,   99,    2, 0x0a /* Public */,
       9,    0,  100,    2, 0x0a /* Public */,
      10,    0,  101,    2, 0x0a /* Public */,
      11,    1,  102,    2, 0x0a /* Public */,
      12,    1,  105,    2, 0x0a /* Public */,
      13,    0,  108,    2, 0x0a /* Public */,
      14,    0,  109,    2, 0x0a /* Public */,
      15,    0,  110,    2, 0x0a /* Public */,
      16,    0,  111,    2, 0x08 /* Private */,

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
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Decrypt_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Decrypt_Dialog *_t = static_cast<Decrypt_Dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->slotCode1(); break;
        case 2: _t->slotCode2(); break;
        case 3: _t->slotCode3(); break;
        case 4: _t->slotCode4(); break;
        case 5: _t->slotCode1_2(); break;
        case 6: _t->slotCode2_2(); break;
        case 7: _t->slotCode3_2(); break;
        case 8: _t->slotCode4_2(); break;
        case 9: _t->slotVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->slotSys((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->slotSign(); break;
        case 12: _t->update(); break;
        case 13: _t->setSysString(); break;
        case 14: _t->on_Btn_More_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Decrypt_Dialog::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Decrypt_Dialog::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Decrypt_Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Decrypt_Dialog.data,
      qt_meta_data_Decrypt_Dialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Decrypt_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Decrypt_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Decrypt_Dialog.stringdata0))
        return static_cast<void*>(const_cast< Decrypt_Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Decrypt_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Decrypt_Dialog::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
