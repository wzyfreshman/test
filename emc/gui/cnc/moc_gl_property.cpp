/****************************************************************************
** Meta object code from reading C++ file 'gl_property.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gl/gl_property.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gl_property.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_gl_property_t {
    QByteArrayData data[14];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_gl_property_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_gl_property_t qt_meta_stringdata_gl_property = {
    {
QT_MOC_LITERAL(0, 0, 11), // "gl_property"
QT_MOC_LITERAL(1, 12, 7), // "gl_init"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 20), // "slotSetRealTimeColor"
QT_MOC_LITERAL(4, 42, 16), // "slotSetFeedColor"
QT_MOC_LITERAL(5, 59, 22), // "slotSetBackGroundColor"
QT_MOC_LITERAL(6, 82, 19), // "slotSetArcFeedColor"
QT_MOC_LITERAL(7, 102, 18), // "slotSetOriginColor"
QT_MOC_LITERAL(8, 121, 20), // "slotSetTraverseColor"
QT_MOC_LITERAL(9, 142, 20), // "slotSetSelectedColor"
QT_MOC_LITERAL(10, 163, 17), // "slotSetLimitColor"
QT_MOC_LITERAL(11, 181, 19), // "on_sure_btn_clicked"
QT_MOC_LITERAL(12, 201, 21), // "on_cancle_btn_clicked"
QT_MOC_LITERAL(13, 223, 20) // "on_reset_btn_clicked"

    },
    "gl_property\0gl_init\0\0slotSetRealTimeColor\0"
    "slotSetFeedColor\0slotSetBackGroundColor\0"
    "slotSetArcFeedColor\0slotSetOriginColor\0"
    "slotSetTraverseColor\0slotSetSelectedColor\0"
    "slotSetLimitColor\0on_sure_btn_clicked\0"
    "on_cancle_btn_clicked\0on_reset_btn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_gl_property[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    1,   75,    2, 0x0a /* Public */,
       4,    1,   78,    2, 0x0a /* Public */,
       5,    1,   81,    2, 0x0a /* Public */,
       6,    1,   84,    2, 0x0a /* Public */,
       7,    1,   87,    2, 0x0a /* Public */,
       8,    1,   90,    2, 0x0a /* Public */,
       9,    1,   93,    2, 0x0a /* Public */,
      10,    1,   96,    2, 0x0a /* Public */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void gl_property::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        gl_property *_t = static_cast<gl_property *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gl_init(); break;
        case 1: _t->slotSetRealTimeColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->slotSetFeedColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 3: _t->slotSetBackGroundColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 4: _t->slotSetArcFeedColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 5: _t->slotSetOriginColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 6: _t->slotSetTraverseColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 7: _t->slotSetSelectedColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 8: _t->slotSetLimitColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 9: _t->on_sure_btn_clicked(); break;
        case 10: _t->on_cancle_btn_clicked(); break;
        case 11: _t->on_reset_btn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject gl_property::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_gl_property.data,
      qt_meta_data_gl_property,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *gl_property::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gl_property::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_gl_property.stringdata0))
        return static_cast<void*>(const_cast< gl_property*>(this));
    return QDialog::qt_metacast(_clname);
}

int gl_property::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
