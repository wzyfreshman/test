/****************************************************************************
** Meta object code from reading C++ file 'cnc_manager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cnc_manager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cnc_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CNC_Manager_t {
    QByteArrayData data[24];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CNC_Manager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CNC_Manager_t qt_meta_stringdata_CNC_Manager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CNC_Manager"
QT_MOC_LITERAL(1, 12, 13), // "jogXplusStart"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "jogYplusStart"
QT_MOC_LITERAL(4, 41, 13), // "jogZplusStart"
QT_MOC_LITERAL(5, 55, 13), // "jogAplusStart"
QT_MOC_LITERAL(6, 69, 14), // "jogXminusStart"
QT_MOC_LITERAL(7, 84, 14), // "jogYminusStart"
QT_MOC_LITERAL(8, 99, 14), // "jogZminusStart"
QT_MOC_LITERAL(9, 114, 14), // "jogAminusStart"
QT_MOC_LITERAL(10, 129, 7), // "jogXEnd"
QT_MOC_LITERAL(11, 137, 7), // "jogYEnd"
QT_MOC_LITERAL(12, 145, 7), // "jogZEnd"
QT_MOC_LITERAL(13, 153, 7), // "jogAEnd"
QT_MOC_LITERAL(14, 161, 8), // "jogIncr1"
QT_MOC_LITERAL(15, 170, 9), // "jogIncr01"
QT_MOC_LITERAL(16, 180, 10), // "jogIncr001"
QT_MOC_LITERAL(17, 191, 11), // "jogIncr0001"
QT_MOC_LITERAL(18, 203, 11), // "jogContinue"
QT_MOC_LITERAL(19, 215, 8), // "jogSpeed"
QT_MOC_LITERAL(20, 224, 9), // "jogHandle"
QT_MOC_LITERAL(21, 234, 17), // "sendSetLightcmdUp"
QT_MOC_LITERAL(22, 252, 12), // "getCncStatus"
QT_MOC_LITERAL(23, 265, 15) // "CNC_STATUS_TYPE"

    },
    "CNC_Manager\0jogXplusStart\0\0jogYplusStart\0"
    "jogZplusStart\0jogAplusStart\0jogXminusStart\0"
    "jogYminusStart\0jogZminusStart\0"
    "jogAminusStart\0jogXEnd\0jogYEnd\0jogZEnd\0"
    "jogAEnd\0jogIncr1\0jogIncr01\0jogIncr001\0"
    "jogIncr0001\0jogContinue\0jogSpeed\0"
    "jogHandle\0sendSetLightcmdUp\0getCncStatus\0"
    "CNC_STATUS_TYPE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CNC_Manager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x0a /* Public */,
       3,    0,  120,    2, 0x0a /* Public */,
       4,    0,  121,    2, 0x0a /* Public */,
       5,    0,  122,    2, 0x0a /* Public */,
       6,    0,  123,    2, 0x0a /* Public */,
       7,    0,  124,    2, 0x0a /* Public */,
       8,    0,  125,    2, 0x0a /* Public */,
       9,    0,  126,    2, 0x0a /* Public */,
      10,    0,  127,    2, 0x0a /* Public */,
      11,    0,  128,    2, 0x0a /* Public */,
      12,    0,  129,    2, 0x0a /* Public */,
      13,    0,  130,    2, 0x0a /* Public */,
      14,    0,  131,    2, 0x0a /* Public */,
      15,    0,  132,    2, 0x0a /* Public */,
      16,    0,  133,    2, 0x0a /* Public */,
      17,    0,  134,    2, 0x0a /* Public */,
      18,    0,  135,    2, 0x0a /* Public */,
      19,    1,  136,    2, 0x0a /* Public */,
      20,    0,  139,    2, 0x0a /* Public */,
      21,    1,  140,    2, 0x0a /* Public */,
      22,    0,  143,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Int, QMetaType::Int,    2,
    0x80000000 | 23,

       0        // eod
};

void CNC_Manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CNC_Manager *_t = static_cast<CNC_Manager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->jogXplusStart(); break;
        case 1: _t->jogYplusStart(); break;
        case 2: _t->jogZplusStart(); break;
        case 3: _t->jogAplusStart(); break;
        case 4: _t->jogXminusStart(); break;
        case 5: _t->jogYminusStart(); break;
        case 6: _t->jogZminusStart(); break;
        case 7: _t->jogAminusStart(); break;
        case 8: _t->jogXEnd(); break;
        case 9: _t->jogYEnd(); break;
        case 10: _t->jogZEnd(); break;
        case 11: _t->jogAEnd(); break;
        case 12: _t->jogIncr1(); break;
        case 13: _t->jogIncr01(); break;
        case 14: _t->jogIncr001(); break;
        case 15: _t->jogIncr0001(); break;
        case 16: _t->jogContinue(); break;
        case 17: _t->jogSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->jogHandle(); break;
        case 19: { int _r = _t->sendSetLightcmdUp((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 20: { CNC_STATUS_TYPE _r = _t->getCncStatus();
            if (_a[0]) *reinterpret_cast< CNC_STATUS_TYPE*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CNC_Manager::staticMetaObject = {
    { &CCD_CncManager::staticMetaObject, qt_meta_stringdata_CNC_Manager.data,
      qt_meta_data_CNC_Manager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CNC_Manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CNC_Manager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CNC_Manager.stringdata0))
        return static_cast<void*>(const_cast< CNC_Manager*>(this));
    return CCD_CncManager::qt_metacast(_clname);
}

int CNC_Manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CCD_CncManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
