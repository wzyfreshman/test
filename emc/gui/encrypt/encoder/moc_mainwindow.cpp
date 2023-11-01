/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[13];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "slotInputCode"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 16), // "slotGenerateCode"
QT_MOC_LITERAL(4, 43, 7), // "slotVer"
QT_MOC_LITERAL(5, 51, 7), // "slotSys"
QT_MOC_LITERAL(6, 59, 8), // "slotInfo"
QT_MOC_LITERAL(7, 68, 11), // "slotInfoday"
QT_MOC_LITERAL(8, 80, 12), // "slotInfohour"
QT_MOC_LITERAL(9, 93, 16), // "slotForeverCheck"
QT_MOC_LITERAL(10, 110, 14), // "slotChangeMode"
QT_MOC_LITERAL(11, 125, 12), // "slotInfoDate"
QT_MOC_LITERAL(12, 138, 4) // "date"

    },
    "MainWindow\0slotInputCode\0\0slotGenerateCode\0"
    "slotVer\0slotSys\0slotInfo\0slotInfoday\0"
    "slotInfohour\0slotForeverCheck\0"
    "slotChangeMode\0slotInfoDate\0date"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       3,    0,   67,    2, 0x0a /* Public */,
       4,    1,   68,    2, 0x0a /* Public */,
       5,    1,   71,    2, 0x0a /* Public */,
       6,    1,   74,    2, 0x0a /* Public */,
       7,    1,   77,    2, 0x0a /* Public */,
       8,    1,   80,    2, 0x0a /* Public */,
       9,    1,   83,    2, 0x0a /* Public */,
      10,    1,   86,    2, 0x0a /* Public */,
      11,    1,   89,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QDate,   12,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotInputCode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->slotGenerateCode(); break;
        case 2: _t->slotVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slotSys((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->slotInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->slotInfoday((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->slotInfohour((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->slotForeverCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slotChangeMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->slotInfoDate((*reinterpret_cast< QDate(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
