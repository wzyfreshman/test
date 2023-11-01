/****************************************************************************
** Meta object code from reading C++ file 'unit_pointsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_pointsdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_pointsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Unit_PointsDialog_t {
    QByteArrayData data[14];
    char stringdata0[247];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Unit_PointsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Unit_PointsDialog_t qt_meta_stringdata_Unit_PointsDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Unit_PointsDialog"
QT_MOC_LITERAL(1, 18, 7), // "showMsg"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 25), // "on_btn_setPoints_released"
QT_MOC_LITERAL(4, 53, 26), // "on_btn_setPoints_2_clicked"
QT_MOC_LITERAL(5, 80, 12), // "set_posZAval"
QT_MOC_LITERAL(6, 93, 12), // "set_posZBval"
QT_MOC_LITERAL(7, 106, 33), // "on_box_points_currentIndexCha..."
QT_MOC_LITERAL(8, 140, 5), // "index"
QT_MOC_LITERAL(9, 146, 35), // "on_box_points_2_currentIndexC..."
QT_MOC_LITERAL(10, 182, 22), // "on_btn_gopoint_clicked"
QT_MOC_LITERAL(11, 205, 13), // "set_Pointxval"
QT_MOC_LITERAL(12, 219, 13), // "set_Pointyval"
QT_MOC_LITERAL(13, 233, 13) // "set_Pointzval"

    },
    "Unit_PointsDialog\0showMsg\0\0"
    "on_btn_setPoints_released\0"
    "on_btn_setPoints_2_clicked\0set_posZAval\0"
    "set_posZBval\0on_box_points_currentIndexChanged\0"
    "index\0on_box_points_2_currentIndexChanged\0"
    "on_btn_gopoint_clicked\0set_Pointxval\0"
    "set_Pointyval\0set_Pointzval"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Unit_PointsDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   74,    2, 0x08 /* Private */,
       4,    0,   75,    2, 0x08 /* Private */,
       5,    0,   76,    2, 0x08 /* Private */,
       6,    0,   77,    2, 0x08 /* Private */,
       7,    1,   78,    2, 0x08 /* Private */,
       9,    1,   81,    2, 0x08 /* Private */,
      10,    0,   84,    2, 0x08 /* Private */,
      11,    0,   85,    2, 0x08 /* Private */,
      12,    0,   86,    2, 0x08 /* Private */,
      13,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Unit_PointsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Unit_PointsDialog *_t = static_cast<Unit_PointsDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->on_btn_setPoints_released(); break;
        case 2: _t->on_btn_setPoints_2_clicked(); break;
        case 3: _t->set_posZAval(); break;
        case 4: _t->set_posZBval(); break;
        case 5: _t->on_box_points_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_box_points_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_btn_gopoint_clicked(); break;
        case 8: _t->set_Pointxval(); break;
        case 9: _t->set_Pointyval(); break;
        case 10: _t->set_Pointzval(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Unit_PointsDialog::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Unit_PointsDialog::showMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Unit_PointsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Unit_PointsDialog.data,
      qt_meta_data_Unit_PointsDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Unit_PointsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Unit_PointsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Unit_PointsDialog.stringdata0))
        return static_cast<void*>(const_cast< Unit_PointsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Unit_PointsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void Unit_PointsDialog::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
