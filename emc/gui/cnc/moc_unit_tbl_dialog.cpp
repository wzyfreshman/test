/****************************************************************************
** Meta object code from reading C++ file 'unit_tbl_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_tbl_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_tbl_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Tbl_Dialog_t {
    QByteArrayData data[18];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Tbl_Dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Tbl_Dialog_t qt_meta_stringdata_Tbl_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Tbl_Dialog"
QT_MOC_LITERAL(1, 11, 14), // "slotItemchange"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(4, 45, 4), // "item"
QT_MOC_LITERAL(5, 50, 12), // "slotLoadTool"
QT_MOC_LITERAL(6, 63, 11), // "slotAddTool"
QT_MOC_LITERAL(7, 75, 14), // "slotSaveToFile"
QT_MOC_LITERAL(8, 90, 14), // "slotRemoveTool"
QT_MOC_LITERAL(9, 105, 6), // "myquit"
QT_MOC_LITERAL(10, 112, 19), // "slotSaveAndLoadDown"
QT_MOC_LITERAL(11, 132, 13), // "changeDiffSet"
QT_MOC_LITERAL(12, 146, 7), // "checked"
QT_MOC_LITERAL(13, 154, 14), // "usingArrayFile"
QT_MOC_LITERAL(14, 169, 4), // "flag"
QT_MOC_LITERAL(15, 174, 12), // "flushDisplay"
QT_MOC_LITERAL(16, 187, 9), // "setNcfile"
QT_MOC_LITERAL(17, 197, 4) // "path"

    },
    "Tbl_Dialog\0slotItemchange\0\0QTableWidgetItem*\0"
    "item\0slotLoadTool\0slotAddTool\0"
    "slotSaveToFile\0slotRemoveTool\0myquit\0"
    "slotSaveAndLoadDown\0changeDiffSet\0"
    "checked\0usingArrayFile\0flag\0flushDisplay\0"
    "setNcfile\0path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Tbl_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    0,   74,    2, 0x0a /* Public */,
       8,    0,   75,    2, 0x0a /* Public */,
       9,    0,   76,    2, 0x0a /* Public */,
      10,    0,   77,    2, 0x0a /* Public */,
      11,    1,   78,    2, 0x0a /* Public */,
      13,    1,   81,    2, 0x0a /* Public */,
      15,    0,   84,    2, 0x0a /* Public */,
      16,    1,   85,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,

       0        // eod
};

void Tbl_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Tbl_Dialog *_t = static_cast<Tbl_Dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotItemchange((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->slotLoadTool(); break;
        case 2: _t->slotAddTool(); break;
        case 3: _t->slotSaveToFile(); break;
        case 4: _t->slotRemoveTool(); break;
        case 5: _t->myquit(); break;
        case 6: _t->slotSaveAndLoadDown(); break;
        case 7: _t->changeDiffSet((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->usingArrayFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->flushDisplay(); break;
        case 10: _t->setNcfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Tbl_Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Tbl_Dialog.data,
      qt_meta_data_Tbl_Dialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Tbl_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Tbl_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Tbl_Dialog.stringdata0))
        return static_cast<void*>(const_cast< Tbl_Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Tbl_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
