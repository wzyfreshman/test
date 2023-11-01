/****************************************************************************
** Meta object code from reading C++ file 'unit_fileeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_fileeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_fileeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyEditor_t {
    QByteArrayData data[5];
    char stringdata0[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyEditor_t qt_meta_stringdata_MyEditor = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyEditor"
QT_MOC_LITERAL(1, 9, 10), // "redoStatus"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 1), // "s"
QT_MOC_LITERAL(4, 23, 10) // "undoStatus"

    },
    "MyEditor\0redoStatus\0\0s\0undoStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void MyEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyEditor *_t = static_cast<MyEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->redoStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->undoStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MyEditor::staticMetaObject = {
    { &unit_codeEditor::staticMetaObject, qt_meta_stringdata_MyEditor.data,
      qt_meta_data_MyEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyEditor.stringdata0))
        return static_cast<void*>(const_cast< MyEditor*>(this));
    return unit_codeEditor::qt_metacast(_clname);
}

int MyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = unit_codeEditor::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_FileEditor_t {
    QByteArrayData data[19];
    char stringdata0[319];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileEditor_t qt_meta_stringdata_FileEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FileEditor"
QT_MOC_LITERAL(1, 11, 7), // "re_load"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 3), // "str"
QT_MOC_LITERAL(4, 24, 11), // "undo_status"
QT_MOC_LITERAL(5, 36, 11), // "redo_status"
QT_MOC_LITERAL(6, 48, 11), // "editingFile"
QT_MOC_LITERAL(7, 60, 5), // "_path"
QT_MOC_LITERAL(8, 66, 8), // "showFile"
QT_MOC_LITERAL(9, 75, 23), // "on_actionUndo_triggered"
QT_MOC_LITERAL(10, 99, 23), // "on_actionRedo_triggered"
QT_MOC_LITERAL(11, 123, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(12, 147, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(13, 171, 23), // "on_actionLoad_triggered"
QT_MOC_LITERAL(14, 195, 23), // "on_actionFind_triggered"
QT_MOC_LITERAL(15, 219, 26), // "on_actionReplace_triggered"
QT_MOC_LITERAL(16, 246, 23), // "on_actionJump_triggered"
QT_MOC_LITERAL(17, 270, 23), // "on_actionDate_triggered"
QT_MOC_LITERAL(18, 294, 24) // "on_actionClose_triggered"

    },
    "FileEditor\0re_load\0\0str\0undo_status\0"
    "redo_status\0editingFile\0_path\0showFile\0"
    "on_actionUndo_triggered\0on_actionRedo_triggered\0"
    "on_actionOpen_triggered\0on_actionSave_triggered\0"
    "on_actionLoad_triggered\0on_actionFind_triggered\0"
    "on_actionReplace_triggered\0"
    "on_actionJump_triggered\0on_actionDate_triggered\0"
    "on_actionClose_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   92,    2, 0x0a /* Public */,
       5,    1,   95,    2, 0x0a /* Public */,
       6,    1,   98,    2, 0x0a /* Public */,
       8,    1,  101,    2, 0x0a /* Public */,
       9,    0,  104,    2, 0x08 /* Private */,
      10,    0,  105,    2, 0x08 /* Private */,
      11,    0,  106,    2, 0x08 /* Private */,
      12,    0,  107,    2, 0x08 /* Private */,
      13,    0,  108,    2, 0x08 /* Private */,
      14,    0,  109,    2, 0x08 /* Private */,
      15,    0,  110,    2, 0x08 /* Private */,
      16,    0,  111,    2, 0x08 /* Private */,
      17,    0,  112,    2, 0x08 /* Private */,
      18,    0,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
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

void FileEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FileEditor *_t = static_cast<FileEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->re_load((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->undo_status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->redo_status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->editingFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->showFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->on_actionUndo_triggered(); break;
        case 6: _t->on_actionRedo_triggered(); break;
        case 7: _t->on_actionOpen_triggered(); break;
        case 8: _t->on_actionSave_triggered(); break;
        case 9: _t->on_actionLoad_triggered(); break;
        case 10: _t->on_actionFind_triggered(); break;
        case 11: _t->on_actionReplace_triggered(); break;
        case 12: _t->on_actionJump_triggered(); break;
        case 13: _t->on_actionDate_triggered(); break;
        case 14: _t->on_actionClose_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FileEditor::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileEditor::re_load)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject FileEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FileEditor.data,
      qt_meta_data_FileEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FileEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FileEditor.stringdata0))
        return static_cast<void*>(const_cast< FileEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FileEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void FileEditor::re_load(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
