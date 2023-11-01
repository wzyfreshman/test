/****************************************************************************
** Meta object code from reading C++ file 'unit_smartrundialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_smartrundialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_smartrundialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SmartRunDialog_t {
    QByteArrayData data[14];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SmartRunDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SmartRunDialog_t qt_meta_stringdata_SmartRunDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SmartRunDialog"
QT_MOC_LITERAL(1, 15, 11), // "runfromline"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 4), // "line"
QT_MOC_LITERAL(4, 33, 7), // "endline"
QT_MOC_LITERAL(5, 41, 11), // "setfilename"
QT_MOC_LITERAL(6, 53, 13), // "settotlalines"
QT_MOC_LITERAL(7, 67, 7), // "runfile"
QT_MOC_LITERAL(8, 75, 11), // "setFilehead"
QT_MOC_LITERAL(9, 87, 3), // "val"
QT_MOC_LITERAL(10, 91, 10), // "setFileend"
QT_MOC_LITERAL(11, 102, 12), // "setStartline"
QT_MOC_LITERAL(12, 115, 10), // "setEndLine"
QT_MOC_LITERAL(13, 126, 10) // "setFastrun"

    },
    "SmartRunDialog\0runfromline\0\0line\0"
    "endline\0setfilename\0settotlalines\0"
    "runfile\0setFilehead\0val\0setFileend\0"
    "setStartline\0setEndLine\0setFastrun"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmartRunDialog[] = {

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
       1,    2,   59,    2, 0x0a /* Public */,
       5,    1,   64,    2, 0x0a /* Public */,
       6,    1,   67,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    1,   71,    2, 0x0a /* Public */,
      10,    1,   74,    2, 0x0a /* Public */,
      11,    1,   77,    2, 0x0a /* Public */,
      12,    1,   80,    2, 0x0a /* Public */,
      13,    1,   83,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void SmartRunDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SmartRunDialog *_t = static_cast<SmartRunDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->runfromline((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->setfilename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->settotlalines((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->runfile(); break;
        case 4: _t->setFilehead((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setFileend((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setStartline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setEndLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setFastrun((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SmartRunDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SmartRunDialog.data,
      qt_meta_data_SmartRunDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SmartRunDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmartRunDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SmartRunDialog.stringdata0))
        return static_cast<void*>(const_cast< SmartRunDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SmartRunDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
