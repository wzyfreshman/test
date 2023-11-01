/****************************************************************************
** Meta object code from reading C++ file 'touch_homedialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/touch_homedialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'touch_homedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HomeDialog_t {
    QByteArrayData data[17];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HomeDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HomeDialog_t qt_meta_stringdata_HomeDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "HomeDialog"
QT_MOC_LITERAL(1, 11, 8), // "send_msg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "str"
QT_MOC_LITERAL(4, 25, 8), // "unhome_x"
QT_MOC_LITERAL(5, 34, 8), // "unhome_y"
QT_MOC_LITERAL(6, 43, 8), // "unhome_z"
QT_MOC_LITERAL(7, 52, 8), // "unhome_a"
QT_MOC_LITERAL(8, 61, 6), // "home_x"
QT_MOC_LITERAL(9, 68, 6), // "home_y"
QT_MOC_LITERAL(10, 75, 6), // "home_z"
QT_MOC_LITERAL(11, 82, 6), // "home_a"
QT_MOC_LITERAL(12, 89, 9), // "home_init"
QT_MOC_LITERAL(13, 99, 9), // "get_datum"
QT_MOC_LITERAL(14, 109, 4), // "axis"
QT_MOC_LITERAL(15, 114, 6), // "Export"
QT_MOC_LITERAL(16, 121, 6) // "Import"

    },
    "HomeDialog\0send_msg\0\0str\0unhome_x\0"
    "unhome_y\0unhome_z\0unhome_a\0home_x\0"
    "home_y\0home_z\0home_a\0home_init\0get_datum\0"
    "axis\0Export\0Import"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomeDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   84,    2, 0x0a /* Public */,
       5,    0,   85,    2, 0x0a /* Public */,
       6,    0,   86,    2, 0x0a /* Public */,
       7,    0,   87,    2, 0x0a /* Public */,
       8,    0,   88,    2, 0x0a /* Public */,
       9,    0,   89,    2, 0x0a /* Public */,
      10,    0,   90,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x0a /* Public */,
      12,    0,   92,    2, 0x0a /* Public */,
      13,    1,   93,    2, 0x0a /* Public */,
      15,    0,   96,    2, 0x0a /* Public */,
      16,    0,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    3,

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
    QMetaType::Double, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HomeDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HomeDialog *_t = static_cast<HomeDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->unhome_x(); break;
        case 2: _t->unhome_y(); break;
        case 3: _t->unhome_z(); break;
        case 4: _t->unhome_a(); break;
        case 5: _t->home_x(); break;
        case 6: _t->home_y(); break;
        case 7: _t->home_z(); break;
        case 8: _t->home_a(); break;
        case 9: _t->home_init(); break;
        case 10: { double _r = _t->get_datum((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 11: _t->Export(); break;
        case 12: _t->Import(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HomeDialog::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HomeDialog::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject HomeDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_HomeDialog.data,
      qt_meta_data_HomeDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HomeDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomeDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HomeDialog.stringdata0))
        return static_cast<void*>(const_cast< HomeDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int HomeDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void HomeDialog::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
