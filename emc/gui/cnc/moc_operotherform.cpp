/****************************************************************************
** Meta object code from reading C++ file 'operotherform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/operotherform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'operotherform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_buildInstaller_t {
    QByteArrayData data[4];
    char stringdata0[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_buildInstaller_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_buildInstaller_t qt_meta_stringdata_buildInstaller = {
    {
QT_MOC_LITERAL(0, 0, 14), // "buildInstaller"
QT_MOC_LITERAL(1, 15, 8), // "finished"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12) // "buildinstall"

    },
    "buildInstaller\0finished\0\0buildinstall"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_buildInstaller[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void buildInstaller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        buildInstaller *_t = static_cast<buildInstaller *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->buildinstall(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (buildInstaller::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&buildInstaller::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject buildInstaller::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_buildInstaller.data,
      qt_meta_data_buildInstaller,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *buildInstaller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *buildInstaller::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_buildInstaller.stringdata0))
        return static_cast<void*>(const_cast< buildInstaller*>(this));
    return QObject::qt_metacast(_clname);
}

int buildInstaller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void buildInstaller::finished(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_OperOtherForm_t {
    QByteArrayData data[15];
    char stringdata0[284];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OperOtherForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OperOtherForm_t qt_meta_stringdata_OperOtherForm = {
    {
QT_MOC_LITERAL(0, 0, 13), // "OperOtherForm"
QT_MOC_LITERAL(1, 14, 7), // "showMsg"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 23), // "on_btn_register_clicked"
QT_MOC_LITERAL(4, 47, 21), // "on_btn_update_clicked"
QT_MOC_LITERAL(5, 69, 21), // "on_btn_backup_clicked"
QT_MOC_LITERAL(6, 91, 21), // "on_btn_debug_released"
QT_MOC_LITERAL(7, 113, 20), // "on_btn_motor_clicked"
QT_MOC_LITERAL(8, 134, 17), // "on_btn_tv_clicked"
QT_MOC_LITERAL(9, 152, 20), // "on_btn_param_clicked"
QT_MOC_LITERAL(10, 173, 23), // "on_btn_shutdonw_clicked"
QT_MOC_LITERAL(11, 197, 23), // "on_btn_keyboard_clicked"
QT_MOC_LITERAL(12, 221, 17), // "on_btn_io_clicked"
QT_MOC_LITERAL(13, 239, 22), // "on_btn_version_clicked"
QT_MOC_LITERAL(14, 262, 21) // "on_btn_reboot_clicked"

    },
    "OperOtherForm\0showMsg\0\0on_btn_register_clicked\0"
    "on_btn_update_clicked\0on_btn_backup_clicked\0"
    "on_btn_debug_released\0on_btn_motor_clicked\0"
    "on_btn_tv_clicked\0on_btn_param_clicked\0"
    "on_btn_shutdonw_clicked\0on_btn_keyboard_clicked\0"
    "on_btn_io_clicked\0on_btn_version_clicked\0"
    "on_btn_reboot_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OperOtherForm[] = {

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
       3,    0,   84,    2, 0x08 /* Private */,
       4,    0,   85,    2, 0x08 /* Private */,
       5,    0,   86,    2, 0x08 /* Private */,
       6,    0,   87,    2, 0x08 /* Private */,
       7,    0,   88,    2, 0x08 /* Private */,
       8,    0,   89,    2, 0x08 /* Private */,
       9,    0,   90,    2, 0x08 /* Private */,
      10,    0,   91,    2, 0x08 /* Private */,
      11,    0,   92,    2, 0x08 /* Private */,
      12,    0,   93,    2, 0x08 /* Private */,
      13,    0,   94,    2, 0x08 /* Private */,
      14,    0,   95,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

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

       0        // eod
};

void OperOtherForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OperOtherForm *_t = static_cast<OperOtherForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->on_btn_register_clicked(); break;
        case 2: _t->on_btn_update_clicked(); break;
        case 3: _t->on_btn_backup_clicked(); break;
        case 4: _t->on_btn_debug_released(); break;
        case 5: _t->on_btn_motor_clicked(); break;
        case 6: _t->on_btn_tv_clicked(); break;
        case 7: _t->on_btn_param_clicked(); break;
        case 8: _t->on_btn_shutdonw_clicked(); break;
        case 9: _t->on_btn_keyboard_clicked(); break;
        case 10: _t->on_btn_io_clicked(); break;
        case 11: _t->on_btn_version_clicked(); break;
        case 12: _t->on_btn_reboot_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (OperOtherForm::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OperOtherForm::showMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject OperOtherForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OperOtherForm.data,
      qt_meta_data_OperOtherForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OperOtherForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OperOtherForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OperOtherForm.stringdata0))
        return static_cast<void*>(const_cast< OperOtherForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int OperOtherForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void OperOtherForm::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
