/****************************************************************************
** Meta object code from reading C++ file 'array.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "array.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'array.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Array[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,    6,    6,    6, 0x0a,
      44,    6,    6,    6, 0x0a,
      56,    6,    6,    6, 0x0a,
      65,    6,    6,    6, 0x0a,
      78,    6,    6,    6, 0x0a,
      96,    6,    6,    6, 0x0a,
     130,  117,    6,    6, 0x0a,
     162,  157,    6,    6, 0x0a,
     183,    6,    6,    6, 0x0a,
     200,    6,    6,    6, 0x0a,
     216,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Array[] = {
    "Array\0\0loadArrayfile(QString)\0"
    "reflushtext()\0cleartext()\0update()\0"
    "buildarray()\0setTotalRowComp()\0"
    "setTotalColumnComp()\0width,heigth\0"
    "setItemSize(double,double)\0text\0"
    "ItemGetInfo(QString)\0arraySlotBuild()\0"
    "setTotalCompx()\0setTotalCompy()\0"
};

void Array::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Array *_t = static_cast<Array *>(_o);
        switch (_id) {
        case 0: _t->loadArrayfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->reflushtext(); break;
        case 2: _t->cleartext(); break;
        case 3: _t->update(); break;
        case 4: _t->buildarray(); break;
        case 5: _t->setTotalRowComp(); break;
        case 6: _t->setTotalColumnComp(); break;
        case 7: _t->setItemSize((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->ItemGetInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->arraySlotBuild(); break;
        case 10: _t->setTotalCompx(); break;
        case 11: _t->setTotalCompy(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Array::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Array::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Array,
      qt_meta_data_Array, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Array::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Array::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Array::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Array))
        return static_cast<void*>(const_cast< Array*>(this));
    return QWidget::qt_metacast(_clname);
}

int Array::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Array::loadArrayfile(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
