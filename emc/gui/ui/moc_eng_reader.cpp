/****************************************************************************
** Meta object code from reading C++ file 'eng_reader.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "decry_gui/eng_reader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eng_reader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_eng_reader[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   11,   11,   11, 0x08,
      45,   11,   11,   11, 0x08,
      72,   63,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_eng_reader[] = {
    "eng_reader\0\0convertDone(QString)\0"
    "readyRead()\0finishExcute(int)\0name,arg\0"
    "startExcute(QString,QStringList&)\0"
};

void eng_reader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        eng_reader *_t = static_cast<eng_reader *>(_o);
        switch (_id) {
        case 0: _t->convertDone((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->readyRead(); break;
        case 2: _t->finishExcute((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->startExcute((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData eng_reader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject eng_reader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_eng_reader,
      qt_meta_data_eng_reader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &eng_reader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *eng_reader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *eng_reader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_eng_reader))
        return static_cast<void*>(const_cast< eng_reader*>(this));
    return QObject::qt_metacast(_clname);
}

int eng_reader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void eng_reader::convertDone(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
