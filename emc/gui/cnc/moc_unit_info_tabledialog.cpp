/****************************************************************************
** Meta object code from reading C++ file 'unit_info_tabledialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_info_tabledialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_info_tabledialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Info_TableDIalog_t {
    QByteArrayData data[9];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Info_TableDIalog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Info_TableDIalog_t qt_meta_stringdata_Info_TableDIalog = {
    {
QT_MOC_LITERAL(0, 0, 16), // "Info_TableDIalog"
QT_MOC_LITERAL(1, 17, 16), // "insert_info_line"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 4), // "type"
QT_MOC_LITERAL(4, 40, 4), // "info"
QT_MOC_LITERAL(5, 45, 8), // "infoTime"
QT_MOC_LITERAL(6, 54, 11), // "clear_table"
QT_MOC_LITERAL(7, 66, 16), // "classify_display"
QT_MOC_LITERAL(8, 83, 21) // "on_btn_select_clicked"

    },
    "Info_TableDIalog\0insert_info_line\0\0"
    "type\0info\0infoTime\0clear_table\0"
    "classify_display\0on_btn_select_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Info_TableDIalog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x0a /* Public */,
       1,    2,   46,    2, 0x2a /* Public | MethodCloned */,
       6,    0,   51,    2, 0x0a /* Public */,
       7,    0,   52,    2, 0x0a /* Public */,
       8,    0,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Info_TableDIalog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Info_TableDIalog *_t = static_cast<Info_TableDIalog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->insert_info_line((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->insert_info_line((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->clear_table(); break;
        case 3: _t->classify_display(); break;
        case 4: _t->on_btn_select_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Info_TableDIalog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Info_TableDIalog.data,
      qt_meta_data_Info_TableDIalog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Info_TableDIalog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Info_TableDIalog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Info_TableDIalog.stringdata0))
        return static_cast<void*>(const_cast< Info_TableDIalog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Info_TableDIalog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
