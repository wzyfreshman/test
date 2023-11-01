/****************************************************************************
** Meta object code from reading C++ file 'best_testfuneditor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "best/best_testfuneditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'best_testfuneditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Best_TestFunEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      45,   40,   19,   19, 0x08,
      87,   40,   19,   19, 0x08,
     129,   40,   19,   19, 0x08,
     172,   40,   19,   19, 0x08,
     216,   19,   19,   19, 0x08,
     236,   19,   19,   19, 0x08,
     267,  259,   19,   19, 0x08,
     298,  259,   19,   19, 0x08,
     328,   19,   19,   19, 0x08,
     350,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Best_TestFunEditor[] = {
    "Best_TestFunEditor\0\0on_ResAdd_clicked()\0"
    "arg1\0on_DestType1_currentIndexChanged(QString)\0"
    "on_DestType2_currentIndexChanged(QString)\0"
    "on_SourceType_currentIndexChanged(QString)\0"
    "on_KeyViewType_currentIndexChanged(QString)\0"
    "on_KeyAdd_clicked()\0on_KeyRemove_clicked()\0"
    "checked\0on_KeyModeSingle_clicked(bool)\0"
    "on_KeyModeCross_clicked(bool)\0"
    "on_Btn_Sure_clicked()\0on_Btn_Cancel_clicked()\0"
};

void Best_TestFunEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Best_TestFunEditor *_t = static_cast<Best_TestFunEditor *>(_o);
        switch (_id) {
        case 0: _t->on_ResAdd_clicked(); break;
        case 1: _t->on_DestType1_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_DestType2_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_SourceType_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->on_KeyViewType_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->on_KeyAdd_clicked(); break;
        case 6: _t->on_KeyRemove_clicked(); break;
        case 7: _t->on_KeyModeSingle_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_KeyModeCross_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_Btn_Sure_clicked(); break;
        case 10: _t->on_Btn_Cancel_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Best_TestFunEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Best_TestFunEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Best_TestFunEditor,
      qt_meta_data_Best_TestFunEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Best_TestFunEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Best_TestFunEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Best_TestFunEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Best_TestFunEditor))
        return static_cast<void*>(const_cast< Best_TestFunEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int Best_TestFunEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
