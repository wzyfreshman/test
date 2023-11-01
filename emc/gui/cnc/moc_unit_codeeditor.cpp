/****************************************************************************
** Meta object code from reading C++ file 'unit_codeeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_codeeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_codeeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyMenu_t {
    QByteArrayData data[1];
    char stringdata0[7];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyMenu_t qt_meta_stringdata_MyMenu = {
    {
QT_MOC_LITERAL(0, 0, 6) // "MyMenu"

    },
    "MyMenu"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MyMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MyMenu::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_MyMenu.data,
      qt_meta_data_MyMenu,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyMenu.stringdata0))
        return static_cast<void*>(const_cast< MyMenu*>(this));
    return QMenu::qt_metacast(_clname);
}

int MyMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_CodeEditor_t {
    QByteArrayData data[16];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CodeEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CodeEditor_t qt_meta_stringdata_CodeEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CodeEditor"
QT_MOC_LITERAL(1, 11, 13), // "ed_clear_data"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 16), // "ed_run_from_line"
QT_MOC_LITERAL(4, 43, 7), // "ed_edit"
QT_MOC_LITERAL(5, 51, 9), // "ed_unload"
QT_MOC_LITERAL(6, 61, 18), // "ed_opengl_highline"
QT_MOC_LITERAL(7, 80, 25), // "updateLineNumberAreaWidth"
QT_MOC_LITERAL(8, 106, 13), // "newBlockCount"
QT_MOC_LITERAL(9, 120, 20), // "highlightCurrentLine"
QT_MOC_LITERAL(10, 141, 20), // "updateLineNumberArea"
QT_MOC_LITERAL(11, 162, 12), // "set_line_pos"
QT_MOC_LITERAL(12, 175, 10), // "clear_data"
QT_MOC_LITERAL(13, 186, 17), // "set_run_from_line"
QT_MOC_LITERAL(14, 204, 12), // "setcursornow"
QT_MOC_LITERAL(15, 217, 8) // "editFile"

    },
    "CodeEditor\0ed_clear_data\0\0ed_run_from_line\0"
    "ed_edit\0ed_unload\0ed_opengl_highline\0"
    "updateLineNumberAreaWidth\0newBlockCount\0"
    "highlightCurrentLine\0updateLineNumberArea\0"
    "set_line_pos\0clear_data\0set_run_from_line\0"
    "setcursornow\0editFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CodeEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       4,    1,   83,    2, 0x06 /* Public */,
       5,    0,   86,    2, 0x06 /* Public */,
       6,    1,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   90,    2, 0x08 /* Private */,
       9,    0,   93,    2, 0x08 /* Private */,
      10,    2,   94,    2, 0x08 /* Private */,
      11,    1,   99,    2, 0x08 /* Private */,
      12,    0,  102,    2, 0x08 /* Private */,
      13,    0,  103,    2, 0x08 /* Private */,
      14,    0,  104,    2, 0x08 /* Private */,
      15,    0,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CodeEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CodeEditor *_t = static_cast<CodeEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ed_clear_data(); break;
        case 1: _t->ed_run_from_line((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ed_edit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->ed_unload(); break;
        case 4: _t->ed_opengl_highline((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->updateLineNumberAreaWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->highlightCurrentLine(); break;
        case 7: _t->updateLineNumberArea((*reinterpret_cast< const QRect(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->set_line_pos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->clear_data(); break;
        case 10: _t->set_run_from_line(); break;
        case 11: _t->setcursornow(); break;
        case 12: _t->editFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CodeEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::ed_clear_data)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CodeEditor::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::ed_run_from_line)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CodeEditor::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::ed_edit)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CodeEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::ed_unload)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CodeEditor::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::ed_opengl_highline)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject CodeEditor::staticMetaObject = {
    { &QPlainTextEdit::staticMetaObject, qt_meta_stringdata_CodeEditor.data,
      qt_meta_data_CodeEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CodeEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CodeEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CodeEditor.stringdata0))
        return static_cast<void*>(const_cast< CodeEditor*>(this));
    return QPlainTextEdit::qt_metacast(_clname);
}

int CodeEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
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
void CodeEditor::ed_clear_data()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CodeEditor::ed_run_from_line(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CodeEditor::ed_edit(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CodeEditor::ed_unload()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void CodeEditor::ed_opengl_highline(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_unit_codeEditor_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_unit_codeEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_unit_codeEditor_t qt_meta_stringdata_unit_codeEditor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "unit_codeEditor"
QT_MOC_LITERAL(1, 16, 12), // "jump_to_line"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "unit_codeEditor\0jump_to_line\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_unit_codeEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void unit_codeEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        unit_codeEditor *_t = static_cast<unit_codeEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->jump_to_line((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (unit_codeEditor::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&unit_codeEditor::jump_to_line)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject unit_codeEditor::staticMetaObject = {
    { &CodeEditor::staticMetaObject, qt_meta_stringdata_unit_codeEditor.data,
      qt_meta_data_unit_codeEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *unit_codeEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *unit_codeEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_unit_codeEditor.stringdata0))
        return static_cast<void*>(const_cast< unit_codeEditor*>(this));
    return CodeEditor::qt_metacast(_clname);
}

int unit_codeEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CodeEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void unit_codeEditor::jump_to_line(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
