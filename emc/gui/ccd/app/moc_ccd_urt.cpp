/****************************************************************************
** Meta object code from reading C++ file 'ccd_urt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ccd_urt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_urt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_Urt_t {
    QByteArrayData data[21];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_Urt_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_Urt_t qt_meta_stringdata_CCD_Urt = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CCD_Urt"
QT_MOC_LITERAL(1, 8, 9), // "readMyCom"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 10), // "detectAxis"
QT_MOC_LITERAL(4, 30, 7), // "sendMsg"
QT_MOC_LITERAL(5, 38, 3), // "cmd"
QT_MOC_LITERAL(6, 42, 3), // "arg"
QT_MOC_LITERAL(7, 46, 1), // "x"
QT_MOC_LITERAL(8, 48, 1), // "y"
QT_MOC_LITERAL(9, 50, 1), // "a"
QT_MOC_LITERAL(10, 52, 4), // "ex_x"
QT_MOC_LITERAL(11, 57, 9), // "safeWrite"
QT_MOC_LITERAL(12, 67, 5), // "char*"
QT_MOC_LITERAL(13, 73, 3), // "buf"
QT_MOC_LITERAL(14, 77, 4), // "size"
QT_MOC_LITERAL(15, 82, 13), // "handleCommand"
QT_MOC_LITERAL(16, 96, 11), // "QByteArray&"
QT_MOC_LITERAL(17, 108, 5), // "bytes"
QT_MOC_LITERAL(18, 114, 10), // "m91Respond"
QT_MOC_LITERAL(19, 125, 3), // "msg"
QT_MOC_LITERAL(20, 129, 3) // "res"

    },
    "CCD_Urt\0readMyCom\0\0detectAxis\0sendMsg\0"
    "cmd\0arg\0x\0y\0a\0ex_x\0safeWrite\0char*\0"
    "buf\0size\0handleCommand\0QByteArray&\0"
    "bytes\0m91Respond\0msg\0res"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_Urt[] = {

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
       3,    0,   50,    2, 0x08 /* Private */,
       4,    6,   51,    2, 0x08 /* Private */,
       4,    5,   64,    2, 0x28 /* Private | MethodCloned */,
      11,    2,   75,    2, 0x08 /* Private */,
      15,    1,   80,    2, 0x08 /* Private */,
      18,    2,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,    9,
    QMetaType::Bool, 0x80000000 | 12, QMetaType::Int,   13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   19,   20,

       0        // eod
};

void CCD_Urt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_Urt *_t = static_cast<CCD_Urt *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readMyCom(); break;
        case 1: _t->detectAxis(); break;
        case 2: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 3: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 4: { bool _r = _t->safeWrite((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: _t->handleCommand((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->m91Respond((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject CCD_Urt::staticMetaObject = {
    { &CCD_CncManager::staticMetaObject, qt_meta_stringdata_CCD_Urt.data,
      qt_meta_data_CCD_Urt,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_Urt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_Urt::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_Urt.stringdata0))
        return static_cast<void*>(const_cast< CCD_Urt*>(this));
    return CCD_CncManager::qt_metacast(_clname);
}

int CCD_Urt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CCD_CncManager::qt_metacall(_c, _id, _a);
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
