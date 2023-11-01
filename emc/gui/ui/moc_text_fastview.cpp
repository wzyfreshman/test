/****************************************************************************
** Meta object code from reading C++ file 'text_fastview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "decry_gui/text_fastview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'text_fastview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTableView_Fast[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QTableView_Fast[] = {
    "QTableView_Fast\0\0e\0wheelChange(QWheelEvent*)\0"
};

void QTableView_Fast::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTableView_Fast *_t = static_cast<QTableView_Fast *>(_o);
        switch (_id) {
        case 0: _t->wheelChange((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTableView_Fast::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTableView_Fast::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_QTableView_Fast,
      qt_meta_data_QTableView_Fast, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTableView_Fast::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTableView_Fast::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTableView_Fast::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTableView_Fast))
        return static_cast<void*>(const_cast< QTableView_Fast*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int QTableView_Fast::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QTableView_Fast::wheelChange(QWheelEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QScrollBar_Fast[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QScrollBar_Fast[] = {
    "QScrollBar_Fast\0\0wheelChange(QWheelEvent*)\0"
};

void QScrollBar_Fast::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QScrollBar_Fast *_t = static_cast<QScrollBar_Fast *>(_o);
        switch (_id) {
        case 0: _t->wheelChange((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QScrollBar_Fast::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QScrollBar_Fast::staticMetaObject = {
    { &QScrollBar::staticMetaObject, qt_meta_stringdata_QScrollBar_Fast,
      qt_meta_data_QScrollBar_Fast, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QScrollBar_Fast::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QScrollBar_Fast::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QScrollBar_Fast::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QScrollBar_Fast))
        return static_cast<void*>(const_cast< QScrollBar_Fast*>(this));
    return QScrollBar::qt_metacast(_clname);
}

int QScrollBar_Fast::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_Text_FastView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   20,   15,   14, 0x0a,
      58,   53,   15,   14, 0x2a,
      76,   14,   15,   14, 0x2a,
      97,   87,   14,   14, 0x0a,
     129,  124,   14,   14, 0x2a,
     151,   14,   14,   14, 0x0a,
     178,   14,  169,   14, 0x0a,
     192,   14,   14,   14, 0x0a,
     200,   14,   14,   14, 0x0a,
     237,  231,   14,   14, 0x0a,
     254,   14,   14,   14, 0x0a,
     267,   14,   14,   14, 0x0a,
     286,  280,   14,   14, 0x08,
     310,   14,   14,   14, 0x08,
     324,   14,   14,   14, 0x08,
     353,   14,   14,   14, 0x08,
     388,  383,   14,   14, 0x08,
     410,   14,   14,   14, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_Text_FastView[] = {
    "Text_FastView\0\0bool\0file,edit\0"
    "openFile(QString,bool)\0file\0"
    "openFile(QString)\0openFile()\0text,edit\0"
    "setPlainText(QString,bool)\0text\0"
    "setPlainText(QString)\0setReadOnly(bool)\0"
    "QString*\0toPlainText()\0clear()\0"
    "itemChanged(QTableWidgetItem*)\0index\0"
    "setSelected(int)\0insertLine()\0"
    "deleteLine()\0value\0index_valueChanged(int)\0"
    "textChanged()\0doubleClickItem(QModelIndex)\0"
    "activeItem(QTableWidgetItem*)\0edit\0"
    "handleByteArray(bool)\0handleByteArray()\0"
};

void Text_FastView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Text_FastView *_t = static_cast<Text_FastView *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->openFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->openFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->openFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->setPlainText((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->setPlainText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->setReadOnly((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: { QString* _r = _t->toPlainText();
            if (_a[0]) *reinterpret_cast< QString**>(_a[0]) = _r; }  break;
        case 7: _t->clear(); break;
        case 8: _t->itemChanged((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->setSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->insertLine(); break;
        case 11: _t->deleteLine(); break;
        case 12: _t->index_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->textChanged(); break;
        case 14: _t->doubleClickItem((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 15: _t->activeItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 16: _t->handleByteArray((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->handleByteArray(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Text_FastView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Text_FastView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Text_FastView,
      qt_meta_data_Text_FastView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Text_FastView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Text_FastView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Text_FastView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Text_FastView))
        return static_cast<void*>(const_cast< Text_FastView*>(this));
    return QWidget::qt_metacast(_clname);
}

int Text_FastView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
