/****************************************************************************
** Meta object code from reading C++ file 'ccd_doublecamera.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/ccd_doublecamera.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_doublecamera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_DoubleCamera_t {
    QByteArrayData data[16];
    char stringdata0[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_DoubleCamera_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_DoubleCamera_t qt_meta_stringdata_CCD_DoubleCamera = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CCD_DoubleCamera"
QT_MOC_LITERAL(1, 17, 8), // "slotSure"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "slotCancel"
QT_MOC_LITERAL(4, 38, 11), // "slotGetMark"
QT_MOC_LITERAL(5, 50, 6), // "camera"
QT_MOC_LITERAL(6, 57, 5), // "index"
QT_MOC_LITERAL(7, 63, 10), // "slotIdMark"
QT_MOC_LITERAL(8, 74, 16), // "activeMainScreen"
QT_MOC_LITERAL(9, 91, 12), // "slotCncAbort"
QT_MOC_LITERAL(10, 104, 12), // "slotSaveMark"
QT_MOC_LITERAL(11, 117, 3), // "log"
QT_MOC_LITERAL(12, 121, 3), // "str"
QT_MOC_LITERAL(13, 125, 26), // "on_Btn_IdDestMark1_clicked"
QT_MOC_LITERAL(14, 152, 26), // "on_Btn_IdDestMark2_clicked"
QT_MOC_LITERAL(15, 179, 25) // "on_Btn_RecInitPos_clicked"

    },
    "CCD_DoubleCamera\0slotSure\0\0slotCancel\0"
    "slotGetMark\0camera\0index\0slotIdMark\0"
    "activeMainScreen\0slotCncAbort\0"
    "slotSaveMark\0log\0str\0on_Btn_IdDestMark1_clicked\0"
    "on_Btn_IdDestMark2_clicked\0"
    "on_Btn_RecInitPos_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_DoubleCamera[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x0a /* Public */,
       4,    2,   81,    2, 0x0a /* Public */,
       4,    1,   86,    2, 0x2a /* Public | MethodCloned */,
       7,    2,   89,    2, 0x0a /* Public */,
       7,    1,   94,    2, 0x2a /* Public | MethodCloned */,
       8,    0,   97,    2, 0x0a /* Public */,
       9,    0,   98,    2, 0x0a /* Public */,
      10,    0,   99,    2, 0x0a /* Public */,
      11,    1,  100,    2, 0x0a /* Public */,
      13,    0,  103,    2, 0x08 /* Private */,
      14,    0,  104,    2, 0x08 /* Private */,
      15,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CCD_DoubleCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_DoubleCamera *_t = static_cast<CCD_DoubleCamera *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSure(); break;
        case 1: _t->slotCancel(); break;
        case 2: _t->slotGetMark((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->slotGetMark((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotIdMark((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->slotIdMark((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->activeMainScreen(); break;
        case 7: _t->slotCncAbort(); break;
        case 8: _t->slotSaveMark(); break;
        case 9: _t->log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->on_Btn_IdDestMark1_clicked(); break;
        case 11: _t->on_Btn_IdDestMark2_clicked(); break;
        case 12: _t->on_Btn_RecInitPos_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CCD_DoubleCamera::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CCD_DoubleCamera.data,
      qt_meta_data_CCD_DoubleCamera,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_DoubleCamera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_DoubleCamera::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_DoubleCamera.stringdata0))
        return static_cast<void*>(const_cast< CCD_DoubleCamera*>(this));
    if (!strcmp(_clname, "CCD_MarksFace"))
        return static_cast< CCD_MarksFace*>(const_cast< CCD_DoubleCamera*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< CCD_DoubleCamera*>(this));
    return QDialog::qt_metacast(_clname);
}

int CCD_DoubleCamera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
