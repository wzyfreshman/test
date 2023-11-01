/****************************************************************************
** Meta object code from reading C++ file 'backup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Restore_t {
    QByteArrayData data[4];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Restore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Restore_t qt_meta_stringdata_Restore = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Restore"
QT_MOC_LITERAL(1, 8, 13), // "restoreFinish"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 12) // "startRestore"

    },
    "Restore\0restoreFinish\0\0startRestore"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Restore[] = {

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

void Restore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Restore *_t = static_cast<Restore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->restoreFinish((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->startRestore(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Restore::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Restore::restoreFinish)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Restore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Restore.data,
      qt_meta_data_Restore,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Restore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Restore::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Restore.stringdata0))
        return static_cast<void*>(const_cast< Restore*>(this));
    return QObject::qt_metacast(_clname);
}

int Restore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Restore::restoreFinish(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_BuildPackage_t {
    QByteArrayData data[4];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BuildPackage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BuildPackage_t qt_meta_stringdata_BuildPackage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "BuildPackage"
QT_MOC_LITERAL(1, 13, 18), // "BuildPackageFinish"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 17) // "startBuildPackage"

    },
    "BuildPackage\0BuildPackageFinish\0\0"
    "startBuildPackage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BuildPackage[] = {

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

void BuildPackage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BuildPackage *_t = static_cast<BuildPackage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->BuildPackageFinish((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->startBuildPackage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BuildPackage::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BuildPackage::BuildPackageFinish)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BuildPackage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BuildPackage.data,
      qt_meta_data_BuildPackage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BuildPackage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BuildPackage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BuildPackage.stringdata0))
        return static_cast<void*>(const_cast< BuildPackage*>(this));
    return QObject::qt_metacast(_clname);
}

int BuildPackage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void BuildPackage::BuildPackageFinish(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Backup_t {
    QByteArrayData data[9];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Backup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Backup_t qt_meta_stringdata_Backup = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Backup"
QT_MOC_LITERAL(1, 7, 7), // "working"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 7), // "getItem"
QT_MOC_LITERAL(4, 24, 5), // "index"
QT_MOC_LITERAL(5, 30, 6), // "backup"
QT_MOC_LITERAL(6, 37, 13), // "deletePackage"
QT_MOC_LITERAL(7, 51, 7), // "restore"
QT_MOC_LITERAL(8, 59, 7) // "setlock"

    },
    "Backup\0working\0\0getItem\0index\0backup\0"
    "deletePackage\0restore\0setlock"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Backup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   47,    2, 0x0a /* Public */,
       5,    0,   50,    2, 0x0a /* Public */,
       6,    0,   51,    2, 0x0a /* Public */,
       7,    0,   52,    2, 0x0a /* Public */,
       8,    1,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void Backup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Backup *_t = static_cast<Backup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->working((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->getItem((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->backup(); break;
        case 3: _t->deletePackage(); break;
        case 4: _t->restore(); break;
        case 5: _t->setlock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Backup::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Backup::working)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Backup::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Backup.data,
      qt_meta_data_Backup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Backup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Backup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Backup.stringdata0))
        return static_cast<void*>(const_cast< Backup*>(this));
    return QDialog::qt_metacast(_clname);
}

int Backup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Backup::working(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
