/****************************************************************************
** Meta object code from reading C++ file 'ccd_cncmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/ccd_cncmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_cncmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_CncManager_t {
    QByteArrayData data[54];
    char stringdata0[520];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_CncManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_CncManager_t qt_meta_stringdata_CCD_CncManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CCD_CncManager"
QT_MOC_LITERAL(1, 15, 10), // "excuteFile"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "file"
QT_MOC_LITERAL(4, 32, 4), // "mode"
QT_MOC_LITERAL(5, 37, 9), // "excuteCmd"
QT_MOC_LITERAL(6, 47, 1), // "s"
QT_MOC_LITERAL(7, 49, 11), // "changeM71IO"
QT_MOC_LITERAL(8, 61, 11), // "changeM64IO"
QT_MOC_LITERAL(9, 73, 15), // "ioStatusChanged"
QT_MOC_LITERAL(10, 89, 9), // "m91Notify"
QT_MOC_LITERAL(11, 99, 1), // "n"
QT_MOC_LITERAL(12, 101, 4), // "int*"
QT_MOC_LITERAL(13, 106, 3), // "ret"
QT_MOC_LITERAL(14, 110, 10), // "m91Respond"
QT_MOC_LITERAL(15, 121, 3), // "res"
QT_MOC_LITERAL(16, 125, 3), // "len"
QT_MOC_LITERAL(17, 129, 7), // "double*"
QT_MOC_LITERAL(18, 137, 4), // "data"
QT_MOC_LITERAL(19, 142, 12), // "setParameter"
QT_MOC_LITERAL(20, 155, 3), // "var"
QT_MOC_LITERAL(21, 159, 5), // "value"
QT_MOC_LITERAL(22, 165, 12), // "getParameter"
QT_MOC_LITERAL(23, 178, 5), // "abort"
QT_MOC_LITERAL(24, 184, 14), // "getfloodstatus"
QT_MOC_LITERAL(25, 199, 7), // "jogStop"
QT_MOC_LITERAL(26, 207, 4), // "axis"
QT_MOC_LITERAL(27, 212, 7), // "jogCont"
QT_MOC_LITERAL(28, 220, 5), // "speed"
QT_MOC_LITERAL(29, 226, 7), // "jogIncr"
QT_MOC_LITERAL(30, 234, 4), // "incr"
QT_MOC_LITERAL(31, 239, 13), // "jogXplusStart"
QT_MOC_LITERAL(32, 253, 13), // "jogYplusStart"
QT_MOC_LITERAL(33, 267, 13), // "jogZplusStart"
QT_MOC_LITERAL(34, 281, 13), // "jogAplusStart"
QT_MOC_LITERAL(35, 295, 14), // "jogXminusStart"
QT_MOC_LITERAL(36, 310, 14), // "jogYminusStart"
QT_MOC_LITERAL(37, 325, 14), // "jogZminusStart"
QT_MOC_LITERAL(38, 340, 14), // "jogAminusStart"
QT_MOC_LITERAL(39, 355, 7), // "jogXEnd"
QT_MOC_LITERAL(40, 363, 7), // "jogYEnd"
QT_MOC_LITERAL(41, 371, 7), // "jogZEnd"
QT_MOC_LITERAL(42, 379, 7), // "jogAEnd"
QT_MOC_LITERAL(43, 387, 8), // "jogIncr1"
QT_MOC_LITERAL(44, 396, 9), // "jogIncr01"
QT_MOC_LITERAL(45, 406, 10), // "jogIncr001"
QT_MOC_LITERAL(46, 417, 11), // "jogIncr0001"
QT_MOC_LITERAL(47, 429, 11), // "jogContinue"
QT_MOC_LITERAL(48, 441, 8), // "jogSpeed"
QT_MOC_LITERAL(49, 450, 9), // "jogHandle"
QT_MOC_LITERAL(50, 460, 12), // "updateStatus"
QT_MOC_LITERAL(51, 473, 15), // "CNC_STATUS_TYPE"
QT_MOC_LITERAL(52, 489, 17), // "sendSetLightcmdUp"
QT_MOC_LITERAL(53, 507, 12) // "getCncStatus"

    },
    "CCD_CncManager\0excuteFile\0\0file\0mode\0"
    "excuteCmd\0s\0changeM71IO\0changeM64IO\0"
    "ioStatusChanged\0m91Notify\0n\0int*\0ret\0"
    "m91Respond\0res\0len\0double*\0data\0"
    "setParameter\0var\0value\0getParameter\0"
    "abort\0getfloodstatus\0jogStop\0axis\0"
    "jogCont\0speed\0jogIncr\0incr\0jogXplusStart\0"
    "jogYplusStart\0jogZplusStart\0jogAplusStart\0"
    "jogXminusStart\0jogYminusStart\0"
    "jogZminusStart\0jogAminusStart\0jogXEnd\0"
    "jogYEnd\0jogZEnd\0jogAEnd\0jogIncr1\0"
    "jogIncr01\0jogIncr001\0jogIncr0001\0"
    "jogContinue\0jogSpeed\0jogHandle\0"
    "updateStatus\0CNC_STATUS_TYPE\0"
    "sendSetLightcmdUp\0getCncStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_CncManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  214,    2, 0x0a /* Public */,
       5,    1,  219,    2, 0x0a /* Public */,
       7,    2,  222,    2, 0x0a /* Public */,
       8,    2,  227,    2, 0x0a /* Public */,
       9,    2,  232,    2, 0x0a /* Public */,
      10,    1,  237,    2, 0x0a /* Public */,
      10,    2,  240,    2, 0x0a /* Public */,
      14,    1,  245,    2, 0x0a /* Public */,
      14,    4,  248,    2, 0x0a /* Public */,
      14,    3,  257,    2, 0x2a /* Public | MethodCloned */,
      14,    2,  264,    2, 0x2a /* Public | MethodCloned */,
      19,    2,  269,    2, 0x0a /* Public */,
      22,    1,  274,    2, 0x0a /* Public */,
      23,    0,  277,    2, 0x0a /* Public */,
      24,    0,  278,    2, 0x0a /* Public */,
      25,    1,  279,    2, 0x0a /* Public */,
      27,    2,  282,    2, 0x0a /* Public */,
      29,    3,  287,    2, 0x0a /* Public */,
      31,    0,  294,    2, 0x0a /* Public */,
      32,    0,  295,    2, 0x0a /* Public */,
      33,    0,  296,    2, 0x0a /* Public */,
      34,    0,  297,    2, 0x0a /* Public */,
      35,    0,  298,    2, 0x0a /* Public */,
      36,    0,  299,    2, 0x0a /* Public */,
      37,    0,  300,    2, 0x0a /* Public */,
      38,    0,  301,    2, 0x0a /* Public */,
      39,    0,  302,    2, 0x0a /* Public */,
      40,    0,  303,    2, 0x0a /* Public */,
      41,    0,  304,    2, 0x0a /* Public */,
      42,    0,  305,    2, 0x0a /* Public */,
      43,    0,  306,    2, 0x0a /* Public */,
      44,    0,  307,    2, 0x0a /* Public */,
      45,    0,  308,    2, 0x0a /* Public */,
      46,    0,  309,    2, 0x0a /* Public */,
      47,    0,  310,    2, 0x0a /* Public */,
      48,    1,  311,    2, 0x0a /* Public */,
      49,    0,  314,    2, 0x0a /* Public */,
      50,    1,  315,    2, 0x0a /* Public */,
      52,    1,  318,    2, 0x0a /* Public */,
      53,    0,  321,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 17,   11,   15,   16,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   11,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   11,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,   20,   21,
    QMetaType::Double, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   26,
    QMetaType::Int, QMetaType::Int, QMetaType::Double,   26,   28,
    QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,   26,   28,   30,
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
    QMetaType::Void, 0x80000000 | 51,    6,
    QMetaType::Int, QMetaType::Int,    2,
    0x80000000 | 51,

       0        // eod
};

void CCD_CncManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_CncManager *_t = static_cast<CCD_CncManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->excuteFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: _t->excuteCmd((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->changeM71IO((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->changeM64IO((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->ioStatusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->m91Notify((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->m91Notify((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        case 7: _t->m91Respond((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->m91Respond((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< double*(*)>(_a[4]))); break;
        case 9: _t->m91Respond((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->m91Respond((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->setParameter((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 12: { double _r = _t->getParameter((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 13: _t->abort(); break;
        case 14: { int _r = _t->getfloodstatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: { int _r = _t->jogStop((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: { int _r = _t->jogCont((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 17: { int _r = _t->jogIncr((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 18: _t->jogXplusStart(); break;
        case 19: _t->jogYplusStart(); break;
        case 20: _t->jogZplusStart(); break;
        case 21: _t->jogAplusStart(); break;
        case 22: _t->jogXminusStart(); break;
        case 23: _t->jogYminusStart(); break;
        case 24: _t->jogZminusStart(); break;
        case 25: _t->jogAminusStart(); break;
        case 26: _t->jogXEnd(); break;
        case 27: _t->jogYEnd(); break;
        case 28: _t->jogZEnd(); break;
        case 29: _t->jogAEnd(); break;
        case 30: _t->jogIncr1(); break;
        case 31: _t->jogIncr01(); break;
        case 32: _t->jogIncr001(); break;
        case 33: _t->jogIncr0001(); break;
        case 34: _t->jogContinue(); break;
        case 35: _t->jogSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->jogHandle(); break;
        case 37: _t->updateStatus((*reinterpret_cast< CNC_STATUS_TYPE(*)>(_a[1]))); break;
        case 38: { int _r = _t->sendSetLightcmdUp((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 39: { CNC_STATUS_TYPE _r = _t->getCncStatus();
            if (_a[0]) *reinterpret_cast< CNC_STATUS_TYPE*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CCD_CncManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCD_CncManager.data,
      qt_meta_data_CCD_CncManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_CncManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_CncManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_CncManager.stringdata0))
        return static_cast<void*>(const_cast< CCD_CncManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCD_CncManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 40;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
