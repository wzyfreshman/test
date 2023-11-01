/****************************************************************************
** Meta object code from reading C++ file 'touch_filemanagers.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "touch/touch_filemanagers.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'touch_filemanagers.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FileManagers_t {
    QByteArrayData data[22];
    char stringdata0[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileManagers_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileManagers_t qt_meta_stringdata_FileManagers = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FileManagers"
QT_MOC_LITERAL(1, 13, 4), // "Init"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 8), // "sendPath"
QT_MOC_LITERAL(4, 28, 5), // "UDisk"
QT_MOC_LITERAL(5, 34, 4), // "Copy"
QT_MOC_LITERAL(6, 39, 3), // "Cut"
QT_MOC_LITERAL(7, 43, 5), // "Paste"
QT_MOC_LITERAL(8, 49, 6), // "Delete"
QT_MOC_LITERAL(9, 56, 5), // "Enter"
QT_MOC_LITERAL(10, 62, 2), // "Up"
QT_MOC_LITERAL(11, 65, 4), // "Exit"
QT_MOC_LITERAL(12, 70, 5), // "UQuit"
QT_MOC_LITERAL(13, 76, 13), // "setbtnEnabled"
QT_MOC_LITERAL(14, 90, 21), // "on_pushButton_pressed"
QT_MOC_LITERAL(15, 112, 22), // "on_btn_desktop_pressed"
QT_MOC_LITERAL(16, 135, 9), // "boxChange"
QT_MOC_LITERAL(17, 145, 17), // "ItemDoubleClicked"
QT_MOC_LITERAL(18, 163, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(19, 180, 11), // "changeEvent"
QT_MOC_LITERAL(20, 192, 7), // "QEvent*"
QT_MOC_LITERAL(21, 200, 4) // "Root"

    },
    "FileManagers\0Init\0\0sendPath\0UDisk\0"
    "Copy\0Cut\0Paste\0Delete\0Enter\0Up\0Exit\0"
    "UQuit\0setbtnEnabled\0on_pushButton_pressed\0"
    "on_btn_desktop_pressed\0boxChange\0"
    "ItemDoubleClicked\0QListWidgetItem*\0"
    "changeEvent\0QEvent*\0Root"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileManagers[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    1,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  108,    2, 0x0a /* Public */,
       5,    0,  109,    2, 0x0a /* Public */,
       6,    0,  110,    2, 0x0a /* Public */,
       7,    0,  111,    2, 0x0a /* Public */,
       8,    0,  112,    2, 0x0a /* Public */,
       9,    0,  113,    2, 0x0a /* Public */,
      10,    0,  114,    2, 0x0a /* Public */,
      11,    0,  115,    2, 0x0a /* Public */,
      12,    0,  116,    2, 0x0a /* Public */,
      13,    0,  117,    2, 0x08 /* Private */,
      14,    0,  118,    2, 0x08 /* Private */,
      15,    0,  119,    2, 0x08 /* Private */,
      16,    1,  120,    2, 0x08 /* Private */,
      17,    1,  123,    2, 0x09 /* Protected */,
      19,    1,  126,    2, 0x09 /* Protected */,
      21,    0,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

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
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 18,    2,
    QMetaType::Void, 0x80000000 | 20,    2,
    QMetaType::Void,

       0        // eod
};

void FileManagers::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FileManagers *_t = static_cast<FileManagers *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Init(); break;
        case 1: _t->sendPath((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->UDisk(); break;
        case 3: _t->Copy(); break;
        case 4: _t->Cut(); break;
        case 5: _t->Paste(); break;
        case 6: _t->Delete(); break;
        case 7: _t->Enter(); break;
        case 8: _t->Up(); break;
        case 9: _t->Exit(); break;
        case 10: _t->UQuit(); break;
        case 11: _t->setbtnEnabled(); break;
        case 12: _t->on_pushButton_pressed(); break;
        case 13: _t->on_btn_desktop_pressed(); break;
        case 14: _t->boxChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->ItemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 16: _t->changeEvent((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        case 17: _t->Root(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FileManagers::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileManagers::Init)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FileManagers::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileManagers::sendPath)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject FileManagers::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FileManagers.data,
      qt_meta_data_FileManagers,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FileManagers::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileManagers::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FileManagers.stringdata0))
        return static_cast<void*>(const_cast< FileManagers*>(this));
    return QDialog::qt_metacast(_clname);
}

int FileManagers::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void FileManagers::Init()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void FileManagers::sendPath(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
