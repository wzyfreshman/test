/****************************************************************************
** Meta object code from reading C++ file 'bitmap_editor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/imgeditor/bitmap_editor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bitmap_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BitMap_Editor_t {
    QByteArrayData data[12];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BitMap_Editor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BitMap_Editor_t qt_meta_stringdata_BitMap_Editor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BitMap_Editor"
QT_MOC_LITERAL(1, 14, 21), // "on_Btn_Eraser_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "checked"
QT_MOC_LITERAL(4, 45, 30), // "on_Sld_EraserSize_valueChanged"
QT_MOC_LITERAL(5, 76, 5), // "value"
QT_MOC_LITERAL(6, 82, 23), // "on_Btn_FreeDraw_clicked"
QT_MOC_LITERAL(7, 106, 19), // "on_Btn_Undo_clicked"
QT_MOC_LITERAL(8, 126, 19), // "on_Btn_Redo_clicked"
QT_MOC_LITERAL(9, 146, 19), // "on_Btn_Sure_clicked"
QT_MOC_LITERAL(10, 166, 21), // "on_Btn_Cancel_clicked"
QT_MOC_LITERAL(11, 188, 20) // "on_Btn_Reset_clicked"

    },
    "BitMap_Editor\0on_Btn_Eraser_clicked\0"
    "\0checked\0on_Sld_EraserSize_valueChanged\0"
    "value\0on_Btn_FreeDraw_clicked\0"
    "on_Btn_Undo_clicked\0on_Btn_Redo_clicked\0"
    "on_Btn_Sure_clicked\0on_Btn_Cancel_clicked\0"
    "on_Btn_Reset_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BitMap_Editor[] = {

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
       1,    1,   54,    2, 0x08 /* Private */,
       4,    1,   57,    2, 0x08 /* Private */,
       6,    1,   60,    2, 0x08 /* Private */,
       7,    0,   63,    2, 0x08 /* Private */,
       8,    0,   64,    2, 0x08 /* Private */,
       9,    0,   65,    2, 0x08 /* Private */,
      10,    0,   66,    2, 0x08 /* Private */,
      11,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BitMap_Editor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BitMap_Editor *_t = static_cast<BitMap_Editor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_Btn_Eraser_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_Sld_EraserSize_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_Btn_FreeDraw_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_Btn_Undo_clicked(); break;
        case 4: _t->on_Btn_Redo_clicked(); break;
        case 5: _t->on_Btn_Sure_clicked(); break;
        case 6: _t->on_Btn_Cancel_clicked(); break;
        case 7: _t->on_Btn_Reset_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject BitMap_Editor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BitMap_Editor.data,
      qt_meta_data_BitMap_Editor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BitMap_Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BitMap_Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BitMap_Editor.stringdata0))
        return static_cast<void*>(const_cast< BitMap_Editor*>(this));
    return QDialog::qt_metacast(_clname);
}

int BitMap_Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
