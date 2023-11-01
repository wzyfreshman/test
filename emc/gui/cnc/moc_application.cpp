/****************************************************************************
** Meta object code from reading C++ file 'application.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "application.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'application.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_application_t {
    QByteArrayData data[19];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_application_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_application_t qt_meta_stringdata_application = {
    {
QT_MOC_LITERAL(0, 0, 11), // "application"
QT_MOC_LITERAL(1, 12, 7), // "sendMsg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 14), // "on_actionOther"
QT_MOC_LITERAL(4, 36, 22), // "on_actionswitchChinese"
QT_MOC_LITERAL(5, 59, 22), // "on_actionswitchEnglish"
QT_MOC_LITERAL(6, 82, 14), // "set_updateFlag"
QT_MOC_LITERAL(7, 97, 4), // "Flag"
QT_MOC_LITERAL(8, 102, 9), // "updateCNC"
QT_MOC_LITERAL(9, 112, 14), // "version_define"
QT_MOC_LITERAL(10, 127, 11), // "add_menubar"
QT_MOC_LITERAL(11, 139, 18), // "on_btn_rst_clicked"
QT_MOC_LITERAL(12, 158, 26), // "loadPositionManagerFromCam"
QT_MOC_LITERAL(13, 185, 26), // "on_btn_calibration_clicked"
QT_MOC_LITERAL(14, 212, 19), // "on_btn_auto_clicked"
QT_MOC_LITERAL(15, 232, 21), // "on_btn_manual_clicked"
QT_MOC_LITERAL(16, 254, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(17, 276, 23), // "on_btn_axis_par_clicked"
QT_MOC_LITERAL(18, 300, 20) // "on_btn_debug_clicked"

    },
    "application\0sendMsg\0\0on_actionOther\0"
    "on_actionswitchChinese\0on_actionswitchEnglish\0"
    "set_updateFlag\0Flag\0updateCNC\0"
    "version_define\0add_menubar\0"
    "on_btn_rst_clicked\0loadPositionManagerFromCam\0"
    "on_btn_calibration_clicked\0"
    "on_btn_auto_clicked\0on_btn_manual_clicked\0"
    "on_pushButton_clicked\0on_btn_axis_par_clicked\0"
    "on_btn_debug_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_application[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   99,    2, 0x0a /* Public */,
       4,    0,  100,    2, 0x0a /* Public */,
       5,    0,  101,    2, 0x0a /* Public */,
       6,    1,  102,    2, 0x0a /* Public */,
       8,    0,  105,    2, 0x08 /* Private */,
       9,    0,  106,    2, 0x08 /* Private */,
      10,    0,  107,    2, 0x08 /* Private */,
      11,    0,  108,    2, 0x08 /* Private */,
      12,    0,  109,    2, 0x08 /* Private */,
      13,    0,  110,    2, 0x08 /* Private */,
      14,    0,  111,    2, 0x08 /* Private */,
      15,    0,  112,    2, 0x08 /* Private */,
      16,    0,  113,    2, 0x08 /* Private */,
      17,    0,  114,    2, 0x08 /* Private */,
      18,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,
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

       0        // eod
};

void application::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        application *_t = static_cast<application *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->on_actionOther(); break;
        case 2: _t->on_actionswitchChinese(); break;
        case 3: _t->on_actionswitchEnglish(); break;
        case 4: _t->set_updateFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->updateCNC(); break;
        case 6: _t->version_define(); break;
        case 7: _t->add_menubar(); break;
        case 8: _t->on_btn_rst_clicked(); break;
        case 9: _t->loadPositionManagerFromCam(); break;
        case 10: _t->on_btn_calibration_clicked(); break;
        case 11: _t->on_btn_auto_clicked(); break;
        case 12: _t->on_btn_manual_clicked(); break;
        case 13: _t->on_pushButton_clicked(); break;
        case 14: _t->on_btn_axis_par_clicked(); break;
        case 15: _t->on_btn_debug_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (application::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&application::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject application::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_application.data,
      qt_meta_data_application,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *application::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *application::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_application.stringdata0))
        return static_cast<void*>(const_cast< application*>(this));
    return QWidget::qt_metacast(_clname);
}

int application::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void application::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
