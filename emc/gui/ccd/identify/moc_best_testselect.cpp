/****************************************************************************
** Meta object code from reading C++ file 'best_testselect.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "best/best_testselect.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'best_testselect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Best_TestSelect[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      39,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Best_TestSelect[] = {
    "Best_TestSelect\0\0on_Btn_Sure_clicked()\0"
    "on_Btn_Cancel_clicked()\0"
};

void Best_TestSelect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Best_TestSelect *_t = static_cast<Best_TestSelect *>(_o);
        switch (_id) {
        case 0: _t->on_Btn_Sure_clicked(); break;
        case 1: _t->on_Btn_Cancel_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Best_TestSelect::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Best_TestSelect::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Best_TestSelect,
      qt_meta_data_Best_TestSelect, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Best_TestSelect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Best_TestSelect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Best_TestSelect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Best_TestSelect))
        return static_cast<void*>(const_cast< Best_TestSelect*>(this));
    return QDialog::qt_metacast(_clname);
}

int Best_TestSelect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
