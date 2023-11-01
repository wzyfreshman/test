/****************************************************************************
** Meta object code from reading C++ file 'printertest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "print/printertest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'printertest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PrinterTest_t {
    QByteArrayData data[10];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PrinterTest_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PrinterTest_t qt_meta_stringdata_PrinterTest = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PrinterTest"
QT_MOC_LITERAL(1, 12, 22), // "on_btn_printer_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 18), // "on_btn_set_clicked"
QT_MOC_LITERAL(4, 55, 24), // "on_printerType_activated"
QT_MOC_LITERAL(5, 80, 5), // "index"
QT_MOC_LITERAL(6, 86, 26), // "on_show_total_valueChanged"
QT_MOC_LITERAL(7, 113, 4), // "arg1"
QT_MOC_LITERAL(8, 118, 32), // "on_show_unqualified_valueChanged"
QT_MOC_LITERAL(9, 151, 19) // "on_btn_quit_clicked"

    },
    "PrinterTest\0on_btn_printer_clicked\0\0"
    "on_btn_set_clicked\0on_printerType_activated\0"
    "index\0on_show_total_valueChanged\0arg1\0"
    "on_show_unqualified_valueChanged\0"
    "on_btn_quit_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PrinterTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       6,    1,   49,    2, 0x08 /* Private */,
       8,    1,   52,    2, 0x08 /* Private */,
       9,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,

       0        // eod
};

void PrinterTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PrinterTest *_t = static_cast<PrinterTest *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_printer_clicked(); break;
        case 1: _t->on_btn_set_clicked(); break;
        case 2: _t->on_printerType_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_show_total_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_show_unqualified_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_btn_quit_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject PrinterTest::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PrinterTest.data,
      qt_meta_data_PrinterTest,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PrinterTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PrinterTest::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PrinterTest.stringdata0))
        return static_cast<void*>(const_cast< PrinterTest*>(this));
    return QDialog::qt_metacast(_clname);
}

int PrinterTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
