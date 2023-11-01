/****************************************************************************
** Meta object code from reading C++ file 'updater_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "updater_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updater_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Updater_Dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      28,   15,   15,   15, 0x0a,
      55,   46,   15,   15, 0x0a,
      89,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Updater_Dialog[] = {
    "Updater_Dialog\0\0readyRead()\0"
    "finishExcute(int)\0name,arg\0"
    "startExcute(QString,QStringList&)\0"
    "readfile()\0"
};

void Updater_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Updater_Dialog *_t = static_cast<Updater_Dialog *>(_o);
        switch (_id) {
        case 0: _t->readyRead(); break;
        case 1: _t->finishExcute((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->startExcute((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 3: _t->readfile(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Updater_Dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Updater_Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Updater_Dialog,
      qt_meta_data_Updater_Dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Updater_Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Updater_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Updater_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Updater_Dialog))
        return static_cast<void*>(const_cast< Updater_Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Updater_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
