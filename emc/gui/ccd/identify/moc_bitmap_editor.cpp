/****************************************************************************
** Meta object code from reading C++ file 'bitmap_editor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/bitmap_editor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bitmap_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BitMap_Editor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   15,   14,   14, 0x08,
      51,   15,   14,   14, 0x08,
      77,   15,   14,   14, 0x08,
     111,  105,   14,   14, 0x08,
     147,   15,   14,   14, 0x08,
     177,   15,   14,   14, 0x08,
     208,   15,   14,   14, 0x08,
     234,   14,   14,   14, 0x08,
     256,   14,   14,   14, 0x08,
     278,   14,   14,   14, 0x08,
     303,   14,   14,   14, 0x08,
     330,   14,   14,   14, 0x08,
     352,   14,   14,   14, 0x08,
     381,  376,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BitMap_Editor[] = {
    "BitMap_Editor\0\0checked\0"
    "on_Btn_Eraser_clicked(bool)\0"
    "on_Btn_Rect_clicked(bool)\0"
    "on_Btn_Circle_clicked(bool)\0value\0"
    "on_Sld_EraserSize_valueChanged(int)\0"
    "on_Btn_FreeDraw_clicked(bool)\0"
    "on_Btn_SetStartP_clicked(bool)\0"
    "on_Btn_None_clicked(bool)\0"
    "on_Btn_Undo_clicked()\0on_Btn_Redo_clicked()\0"
    "on_Btn_ThinImg_clicked()\0"
    "on_Btn_Generator_clicked()\0"
    "on_Btn_Sure_clicked()\0on_Btn_Cancel_clicked()\0"
    "arg1\0on_Sbx_Sample_valueChanged(int)\0"
};

void BitMap_Editor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BitMap_Editor *_t = static_cast<BitMap_Editor *>(_o);
        switch (_id) {
        case 0: _t->on_Btn_Eraser_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_Btn_Rect_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_Btn_Circle_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_Sld_EraserSize_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_Btn_FreeDraw_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_Btn_SetStartP_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_Btn_None_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_Btn_Undo_clicked(); break;
        case 8: _t->on_Btn_Redo_clicked(); break;
        case 9: _t->on_Btn_ThinImg_clicked(); break;
        case 10: _t->on_Btn_Generator_clicked(); break;
        case 11: _t->on_Btn_Sure_clicked(); break;
        case 12: _t->on_Btn_Cancel_clicked(); break;
        case 13: _t->on_Sbx_Sample_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BitMap_Editor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BitMap_Editor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BitMap_Editor,
      qt_meta_data_BitMap_Editor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BitMap_Editor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BitMap_Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BitMap_Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BitMap_Editor))
        return static_cast<void*>(const_cast< BitMap_Editor*>(this));
    return QDialog::qt_metacast(_clname);
}

int BitMap_Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
