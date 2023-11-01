/****************************************************************************
** Meta object code from reading C++ file 'best_referenceeditor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "best/best_referenceeditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'best_referenceeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Best_ReferenceEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      44,   21,   21,   21, 0x08,
      65,   21,   21,   21, 0x08,
      86,   21,   21,   21, 0x08,
     112,   21,   21,   21, 0x08,
     141,   21,   21,   21, 0x08,
     164,   21,   21,   21, 0x08,
     185,   21,   21,   21, 0x08,
     205,   21,   21,   21, 0x08,
     225,   21,   21,   21, 0x08,
     250,   21,   21,   21, 0x08,
     278,   21,   21,   21, 0x08,
     299,   21,   21,   21, 0x08,
     319,   21,   21,   21, 0x08,
     346,   21,   21,   21, 0x08,
     370,   21,   21,   21, 0x08,
     392,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Best_ReferenceEditor[] = {
    "Best_ReferenceEditor\0\0on_OpenRead_clicked()\0"
    "on_CapRead_clicked()\0on_KeyRead_clicked()\0"
    "on_ResLinerRead_clicked()\0"
    "on_ResInsulateRead_clicked()\0"
    "on_Btn_Reset_clicked()\0on_OpenAvg_clicked()\0"
    "on_CapAvg_clicked()\0on_KeyAvg_clicked()\0"
    "on_ResLinerAvg_clicked()\0"
    "on_ResInsulateAvg_clicked()\0"
    "on_Btn_CCD_clicked()\0on_Btn_Go_clicked()\0"
    "on_Btn_SetZvalue_clicked()\0"
    "on_Btn_Cancel_clicked()\0on_Btn_Sure_clicked()\0"
    "on_Btn_OpenHandle_clicked()\0"
};

void Best_ReferenceEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Best_ReferenceEditor *_t = static_cast<Best_ReferenceEditor *>(_o);
        switch (_id) {
        case 0: _t->on_OpenRead_clicked(); break;
        case 1: _t->on_CapRead_clicked(); break;
        case 2: _t->on_KeyRead_clicked(); break;
        case 3: _t->on_ResLinerRead_clicked(); break;
        case 4: _t->on_ResInsulateRead_clicked(); break;
        case 5: _t->on_Btn_Reset_clicked(); break;
        case 6: _t->on_OpenAvg_clicked(); break;
        case 7: _t->on_CapAvg_clicked(); break;
        case 8: _t->on_KeyAvg_clicked(); break;
        case 9: _t->on_ResLinerAvg_clicked(); break;
        case 10: _t->on_ResInsulateAvg_clicked(); break;
        case 11: _t->on_Btn_CCD_clicked(); break;
        case 12: _t->on_Btn_Go_clicked(); break;
        case 13: _t->on_Btn_SetZvalue_clicked(); break;
        case 14: _t->on_Btn_Cancel_clicked(); break;
        case 15: _t->on_Btn_Sure_clicked(); break;
        case 16: _t->on_Btn_OpenHandle_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Best_ReferenceEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Best_ReferenceEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Best_ReferenceEditor,
      qt_meta_data_Best_ReferenceEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Best_ReferenceEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Best_ReferenceEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Best_ReferenceEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Best_ReferenceEditor))
        return static_cast<void*>(const_cast< Best_ReferenceEditor*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< Best_ReferenceEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int Best_ReferenceEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
