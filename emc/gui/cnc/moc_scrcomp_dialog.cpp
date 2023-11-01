/****************************************************************************
** Meta object code from reading C++ file 'scrcomp_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/scrcomp_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scrcomp_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScrComp_Dialog_t {
    QByteArrayData data[11];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScrComp_Dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScrComp_Dialog_t qt_meta_stringdata_ScrComp_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ScrComp_Dialog"
QT_MOC_LITERAL(1, 15, 8), // "slotOpen"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "current"
QT_MOC_LITERAL(4, 33, 1), // "i"
QT_MOC_LITERAL(5, 35, 18), // "on_btn_not_clicked"
QT_MOC_LITERAL(6, 54, 8), // "openFile"
QT_MOC_LITERAL(7, 63, 4), // "path"
QT_MOC_LITERAL(8, 68, 21), // "on_btn_import_clicked"
QT_MOC_LITERAL(9, 90, 23), // "on_useScrewComp_clicked"
QT_MOC_LITERAL(10, 114, 20) // "on_btn_close_clicked"

    },
    "ScrComp_Dialog\0slotOpen\0\0current\0i\0"
    "on_btn_not_clicked\0openFile\0path\0"
    "on_btn_import_clicked\0on_useScrewComp_clicked\0"
    "on_btn_close_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScrComp_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    1,   50,    2, 0x08 /* Private */,
       5,    0,   53,    2, 0x08 /* Private */,
       6,    1,   54,    2, 0x08 /* Private */,
       8,    0,   57,    2, 0x08 /* Private */,
       9,    0,   58,    2, 0x08 /* Private */,
      10,    0,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ScrComp_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScrComp_Dialog *_t = static_cast<ScrComp_Dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOpen(); break;
        case 1: _t->current((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_btn_not_clicked(); break;
        case 3: { int _r = _t->openFile((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: _t->on_btn_import_clicked(); break;
        case 5: _t->on_useScrewComp_clicked(); break;
        case 6: _t->on_btn_close_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject ScrComp_Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ScrComp_Dialog.data,
      qt_meta_data_ScrComp_Dialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ScrComp_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScrComp_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ScrComp_Dialog.stringdata0))
        return static_cast<void*>(const_cast< ScrComp_Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ScrComp_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
