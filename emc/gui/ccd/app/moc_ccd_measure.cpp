/****************************************************************************
** Meta object code from reading C++ file 'ccd_measure.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/ccd_measure.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_measure.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_Measure_t {
    QByteArrayData data[11];
    char stringdata0[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_Measure_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_Measure_t qt_meta_stringdata_CCD_Measure = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CCD_Measure"
QT_MOC_LITERAL(1, 12, 17), // "on_rBtn_1_clicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "on_rBtn_2_clicked"
QT_MOC_LITERAL(4, 49, 17), // "on_rBtn_3_clicked"
QT_MOC_LITERAL(5, 67, 17), // "on_rBtn_4_clicked"
QT_MOC_LITERAL(6, 85, 26), // "on_Btn_cleanWidget_clicked"
QT_MOC_LITERAL(7, 112, 23), // "on_Btn_cleanMsg_clicked"
QT_MOC_LITERAL(8, 136, 31), // "on_Lbl_camera_rate_valueChanged"
QT_MOC_LITERAL(9, 168, 4), // "arg1"
QT_MOC_LITERAL(10, 173, 18) // "calculateLineSpace"

    },
    "CCD_Measure\0on_rBtn_1_clicked\0\0"
    "on_rBtn_2_clicked\0on_rBtn_3_clicked\0"
    "on_rBtn_4_clicked\0on_Btn_cleanWidget_clicked\0"
    "on_Btn_cleanMsg_clicked\0"
    "on_Lbl_camera_rate_valueChanged\0arg1\0"
    "calculateLineSpace"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_Measure[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    1,   60,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void,

       0        // eod
};

void CCD_Measure::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_Measure *_t = static_cast<CCD_Measure *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_rBtn_1_clicked(); break;
        case 1: _t->on_rBtn_2_clicked(); break;
        case 2: _t->on_rBtn_3_clicked(); break;
        case 3: _t->on_rBtn_4_clicked(); break;
        case 4: _t->on_Btn_cleanWidget_clicked(); break;
        case 5: _t->on_Btn_cleanMsg_clicked(); break;
        case 6: _t->on_Lbl_camera_rate_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->calculateLineSpace(); break;
        default: ;
        }
    }
}

const QMetaObject CCD_Measure::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CCD_Measure.data,
      qt_meta_data_CCD_Measure,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_Measure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_Measure::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_Measure.stringdata0))
        return static_cast<void*>(const_cast< CCD_Measure*>(this));
    if (!strcmp(_clname, "CCD_MarksFace"))
        return static_cast< CCD_MarksFace*>(const_cast< CCD_Measure*>(this));
    return QDialog::qt_metacast(_clname);
}

int CCD_Measure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
