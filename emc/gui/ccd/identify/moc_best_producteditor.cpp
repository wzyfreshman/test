/****************************************************************************
** Meta object code from reading C++ file 'best_producteditor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "best/best_producteditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'best_producteditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Best_ProductEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   20,   19,   19, 0x08,
      57,   20,   19,   19, 0x08,
      89,   20,   19,   19, 0x08,
     121,   20,   19,   19, 0x08,
     153,   20,   19,   19, 0x08,
     185,   19,   19,   19, 0x08,
     207,   19,   19,   19, 0x08,
     232,  227,   19,   19, 0x08,
     277,  269,   19,   19, 0x08,
     305,   19,   19,   19, 0x08,
     330,   19,   19,   19, 0x08,
     355,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Best_ProductEditor[] = {
    "Best_ProductEditor\0\0item\0"
    "itemChangedX(QTableWidgetItem*)\0"
    "itemChangedY(QTableWidgetItem*)\0"
    "itemChangedK(QTableWidgetItem*)\0"
    "itemChangedF(QTableWidgetItem*)\0"
    "itemChangedG(QTableWidgetItem*)\0"
    "on_Btn_Next_clicked()\0on_Btn_Up_clicked()\0"
    "arg1\0on_Product_Name_textChanged(QString)\0"
    "checked\0on_Ck_AutoAdd_clicked(bool)\0"
    "on_Btn_Cancel2_clicked()\0"
    "on_Btn_Cancel1_clicked()\0on_Btn_Sure_clicked()\0"
};

void Best_ProductEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Best_ProductEditor *_t = static_cast<Best_ProductEditor *>(_o);
        switch (_id) {
        case 0: _t->itemChangedX((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->itemChangedY((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->itemChangedK((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->itemChangedF((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->itemChangedG((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->on_Btn_Next_clicked(); break;
        case 6: _t->on_Btn_Up_clicked(); break;
        case 7: _t->on_Product_Name_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_Ck_AutoAdd_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_Btn_Cancel2_clicked(); break;
        case 10: _t->on_Btn_Cancel1_clicked(); break;
        case 11: _t->on_Btn_Sure_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Best_ProductEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Best_ProductEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Best_ProductEditor,
      qt_meta_data_Best_ProductEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Best_ProductEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Best_ProductEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Best_ProductEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Best_ProductEditor))
        return static_cast<void*>(const_cast< Best_ProductEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int Best_ProductEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
