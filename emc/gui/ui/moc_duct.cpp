/****************************************************************************
** Meta object code from reading C++ file 'duct.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "duct.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duct.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Duct[] = {

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
       6,    5,    5,    5, 0x08,
      20,    5,    5,    5, 0x08,
      41,   34,    5,    5, 0x08,
      59,    5,    5,    5, 0x08,
      74,    5,    5,    5, 0x08,
      90,    5,    5,    5, 0x08,
     133,    5,    5,    5, 0x08,
     144,    5,    5,    5, 0x08,
     164,    5,    5,    5, 0x08,
     193,    5,    5,    5, 0x08,
     216,    5,    5,    5, 0x08,
     240,    5,    5,    5, 0x08,
     266,    5,    5,    5, 0x08,
     295,    5,    5,    5, 0x08,
     322,    5,    5,    5, 0x08,
     350,    5,    5,    5, 0x08,
     374,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Duct[] = {
    "Duct\0\0sendMessage()\0readMessage()\0"
    "isSend\0showMessage(bool)\0conectServer()\0"
    "havedConected()\0"
    "displayError(QAbstractSocket::SocketError)\0"
    "openFile()\0startTransferFile()\0"
    "updateClientProgress(qint64)\0"
    "updateServerProgress()\0on_openButton_clicked()\0"
    "on_conectButton_clicked()\0"
    "on_disconectButton_clicked()\0"
    "on_sendMesButton_clicked()\0"
    "on_sendFileButton_clicked()\0"
    "on_quitButton_clicked()\0"
    "on_textBrowser_textChanged()\0"
};

void Duct::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Duct *_t = static_cast<Duct *>(_o);
        switch (_id) {
        case 0: _t->sendMessage(); break;
        case 1: _t->readMessage(); break;
        case 2: _t->showMessage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->conectServer(); break;
        case 4: _t->havedConected(); break;
        case 5: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: _t->openFile(); break;
        case 7: _t->startTransferFile(); break;
        case 8: _t->updateClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 9: _t->updateServerProgress(); break;
        case 10: _t->on_openButton_clicked(); break;
        case 11: _t->on_conectButton_clicked(); break;
        case 12: _t->on_disconectButton_clicked(); break;
        case 13: _t->on_sendMesButton_clicked(); break;
        case 14: _t->on_sendFileButton_clicked(); break;
        case 15: _t->on_quitButton_clicked(); break;
        case 16: _t->on_textBrowser_textChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Duct::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Duct::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Duct,
      qt_meta_data_Duct, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Duct::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Duct::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Duct::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Duct))
        return static_cast<void*>(const_cast< Duct*>(this));
    return QDialog::qt_metacast(_clname);
}

int Duct::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
