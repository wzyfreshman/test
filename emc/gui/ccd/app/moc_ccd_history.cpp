/****************************************************************************
** Meta object code from reading C++ file 'ccd_history.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/ccd_history.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_history.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_History_t {
    QByteArrayData data[8];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_History_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_History_t qt_meta_stringdata_CCD_History = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CCD_History"
QT_MOC_LITERAL(1, 12, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 30), // "on_imgType_currentIndexChanged"
QT_MOC_LITERAL(4, 66, 5), // "index"
QT_MOC_LITERAL(5, 72, 20), // "on_Btn_clear_clicked"
QT_MOC_LITERAL(6, 93, 22), // "on_Btn_saveImg_clicked"
QT_MOC_LITERAL(7, 116, 24) // "on_Btn_resetview_clicked"

    },
    "CCD_History\0on_pushButton_clicked\0\0"
    "on_imgType_currentIndexChanged\0index\0"
    "on_Btn_clear_clicked\0on_Btn_saveImg_clicked\0"
    "on_Btn_resetview_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_History[] = {

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
       1,    0,   39,    2, 0x08 /* Private */,
       3,    1,   40,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CCD_History::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_History *_t = static_cast<CCD_History *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_imgType_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_Btn_clear_clicked(); break;
        case 3: _t->on_Btn_saveImg_clicked(); break;
        case 4: _t->on_Btn_resetview_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CCD_History::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CCD_History.data,
      qt_meta_data_CCD_History,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_History::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_History::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_History.stringdata0))
        return static_cast<void*>(const_cast< CCD_History*>(this));
    if (!strcmp(_clname, "CCD_MarksFace"))
        return static_cast< CCD_MarksFace*>(const_cast< CCD_History*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< CCD_History*>(this));
    return QDialog::qt_metacast(_clname);
}

int CCD_History::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
