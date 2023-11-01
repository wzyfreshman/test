/****************************************************************************
** Meta object code from reading C++ file 'mywidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mywidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyLabel_t {
    QByteArrayData data[4];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyLabel_t qt_meta_stringdata_MyLabel = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MyLabel"
QT_MOC_LITERAL(1, 8, 7), // "clicked"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 9) // "leftClick"

    },
    "MyLabel\0clicked\0\0leftClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyLabel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    1,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MyLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyLabel *_t = static_cast<MyLabel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->leftClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyLabel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyLabel::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyLabel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyLabel::leftClick)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MyLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MyLabel.data,
      qt_meta_data_MyLabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyLabel.stringdata0))
        return static_cast<void*>(const_cast< MyLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int MyLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MyLabel::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MyLabel::leftClick(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_MySlider_t {
    QByteArrayData data[4];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MySlider_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MySlider_t qt_meta_stringdata_MySlider = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MySlider"
QT_MOC_LITERAL(1, 9, 12), // "value_change"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 6) // "setNum"

    },
    "MySlider\0value_change\0\0setNum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MySlider[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void MySlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MySlider *_t = static_cast<MySlider *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->value_change(); break;
        case 1: _t->setNum((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MySlider::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySlider::value_change)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MySlider::staticMetaObject = {
    { &QwtSlider::staticMetaObject, qt_meta_stringdata_MySlider.data,
      qt_meta_data_MySlider,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MySlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySlider::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MySlider.stringdata0))
        return static_cast<void*>(const_cast< MySlider*>(this));
    return QwtSlider::qt_metacast(_clname);
}

int MySlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MySlider::value_change()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_PassModify_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PassModify_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PassModify_t qt_meta_stringdata_PassModify = {
    {
QT_MOC_LITERAL(0, 0, 10), // "PassModify"
QT_MOC_LITERAL(1, 11, 10), // "codeModify"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "PassModify\0codeModify\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PassModify[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int,

       0        // eod
};

void PassModify::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PassModify *_t = static_cast<PassModify *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->codeModify();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject PassModify::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PassModify.data,
      qt_meta_data_PassModify,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PassModify::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PassModify::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PassModify.stringdata0))
        return static_cast<void*>(const_cast< PassModify*>(this));
    return QDialog::qt_metacast(_clname);
}

int PassModify::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_GETPassWord_t {
    QByteArrayData data[6];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GETPassWord_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GETPassWord_t qt_meta_stringdata_GETPassWord = {
    {
QT_MOC_LITERAL(0, 0, 11), // "GETPassWord"
QT_MOC_LITERAL(1, 12, 14), // "ensurePassWord"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "modifycode"
QT_MOC_LITERAL(4, 39, 11), // "getPasscode"
QT_MOC_LITERAL(5, 51, 11) // "getKeyboard"

    },
    "GETPassWord\0ensurePassWord\0\0modifycode\0"
    "getPasscode\0getKeyboard"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GETPassWord[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,

       0        // eod
};

void GETPassWord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GETPassWord *_t = static_cast<GETPassWord *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->ensurePassWord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->modifycode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->getPasscode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->getKeyboard();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject GETPassWord::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GETPassWord.data,
      qt_meta_data_GETPassWord,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GETPassWord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GETPassWord::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GETPassWord.stringdata0))
        return static_cast<void*>(const_cast< GETPassWord*>(this));
    return QDialog::qt_metacast(_clname);
}

int GETPassWord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_PassWord_t {
    QByteArrayData data[1];
    char stringdata0[9];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PassWord_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PassWord_t qt_meta_stringdata_PassWord = {
    {
QT_MOC_LITERAL(0, 0, 8) // "PassWord"

    },
    "PassWord"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PassWord[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void PassWord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PassWord::staticMetaObject = {
    { &QInputDialog::staticMetaObject, qt_meta_stringdata_PassWord.data,
      qt_meta_data_PassWord,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PassWord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PassWord::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PassWord.stringdata0))
        return static_cast<void*>(const_cast< PassWord*>(this));
    return QInputDialog::qt_metacast(_clname);
}

int PassWord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QInputDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_myFindWdiget_t {
    QByteArrayData data[6];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myFindWdiget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myFindWdiget_t qt_meta_stringdata_myFindWdiget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "myFindWdiget"
QT_MOC_LITERAL(1, 13, 7), // "findStr"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "backwardfunc"
QT_MOC_LITERAL(4, 35, 13), // "wholewordfunc"
QT_MOC_LITERAL(5, 49, 17) // "casesensitivefunc"

    },
    "myFindWdiget\0findStr\0\0backwardfunc\0"
    "wholewordfunc\0casesensitivefunc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myFindWdiget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    1,   35,    2, 0x0a /* Public */,
       4,    1,   38,    2, 0x0a /* Public */,
       5,    1,   41,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void myFindWdiget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myFindWdiget *_t = static_cast<myFindWdiget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->findStr(); break;
        case 1: _t->backwardfunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->wholewordfunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->casesensitivefunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject myFindWdiget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_myFindWdiget.data,
      qt_meta_data_myFindWdiget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *myFindWdiget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myFindWdiget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_myFindWdiget.stringdata0))
        return static_cast<void*>(const_cast< myFindWdiget*>(this));
    return QDialog::qt_metacast(_clname);
}

int myFindWdiget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_myReplaceWdiget_t {
    QByteArrayData data[8];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myReplaceWdiget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myReplaceWdiget_t qt_meta_stringdata_myReplaceWdiget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "myReplaceWdiget"
QT_MOC_LITERAL(1, 16, 10), // "replaceStr"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "replaceAll"
QT_MOC_LITERAL(4, 39, 8), // "jumpNext"
QT_MOC_LITERAL(5, 48, 12), // "backwardfunc"
QT_MOC_LITERAL(6, 61, 13), // "wholewordfunc"
QT_MOC_LITERAL(7, 75, 17) // "casesensitivefunc"

    },
    "myReplaceWdiget\0replaceStr\0\0replaceAll\0"
    "jumpNext\0backwardfunc\0wholewordfunc\0"
    "casesensitivefunc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myReplaceWdiget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    1,   47,    2, 0x0a /* Public */,
       6,    1,   50,    2, 0x0a /* Public */,
       7,    1,   53,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void myReplaceWdiget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myReplaceWdiget *_t = static_cast<myReplaceWdiget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->replaceStr(); break;
        case 1: _t->replaceAll(); break;
        case 2: _t->jumpNext(); break;
        case 3: _t->backwardfunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->wholewordfunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->casesensitivefunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject myReplaceWdiget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_myReplaceWdiget.data,
      qt_meta_data_myReplaceWdiget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *myReplaceWdiget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myReplaceWdiget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_myReplaceWdiget.stringdata0))
        return static_cast<void*>(const_cast< myReplaceWdiget*>(this));
    return QDialog::qt_metacast(_clname);
}

int myReplaceWdiget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
struct qt_meta_stringdata_myLock_t {
    QByteArrayData data[3];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myLock_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myLock_t qt_meta_stringdata_myLock = {
    {
QT_MOC_LITERAL(0, 0, 6), // "myLock"
QT_MOC_LITERAL(1, 7, 7), // "timeout"
QT_MOC_LITERAL(2, 15, 0) // ""

    },
    "myLock\0timeout\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myLock[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void myLock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myLock *_t = static_cast<myLock *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timeout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject myLock::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_myLock.data,
      qt_meta_data_myLock,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *myLock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myLock::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_myLock.stringdata0))
        return static_cast<void*>(const_cast< myLock*>(this));
    return QObject::qt_metacast(_clname);
}

int myLock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_SetFeed_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetFeed_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetFeed_t qt_meta_stringdata_SetFeed = {
    {
QT_MOC_LITERAL(0, 0, 7), // "SetFeed"
QT_MOC_LITERAL(1, 8, 13), // "ensureSetFeed"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "SetFeed\0ensureSetFeed\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetFeed[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void SetFeed::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SetFeed *_t = static_cast<SetFeed *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensureSetFeed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SetFeed::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SetFeed.data,
      qt_meta_data_SetFeed,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SetFeed::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetFeed::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SetFeed.stringdata0))
        return static_cast<void*>(const_cast< SetFeed*>(this));
    return QDialog::qt_metacast(_clname);
}

int SetFeed::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MyInput_t {
    QByteArrayData data[24];
    char stringdata0[361];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyInput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyInput_t qt_meta_stringdata_MyInput = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MyInput"
QT_MOC_LITERAL(1, 8, 12), // "ensureSetVal"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 15), // "btn_one_clicked"
QT_MOC_LITERAL(4, 38, 15), // "btn_two_clicked"
QT_MOC_LITERAL(5, 54, 17), // "btn_three_clicked"
QT_MOC_LITERAL(6, 72, 16), // "btn_four_clicked"
QT_MOC_LITERAL(7, 89, 16), // "btn_five_clicked"
QT_MOC_LITERAL(8, 106, 15), // "btn_six_clicked"
QT_MOC_LITERAL(9, 122, 17), // "btn_eight_clicked"
QT_MOC_LITERAL(10, 140, 17), // "btn_seven_clicked"
QT_MOC_LITERAL(11, 158, 16), // "btn_nine_clicked"
QT_MOC_LITERAL(12, 175, 16), // "btn_zero_clicked"
QT_MOC_LITERAL(13, 192, 15), // "btn_dec_clicked"
QT_MOC_LITERAL(14, 208, 15), // "btn_neg_clicked"
QT_MOC_LITERAL(15, 224, 13), // "btn_a_clicked"
QT_MOC_LITERAL(16, 238, 13), // "btn_b_clicked"
QT_MOC_LITERAL(17, 252, 13), // "btn_c_clicked"
QT_MOC_LITERAL(18, 266, 13), // "btn_d_clicked"
QT_MOC_LITERAL(19, 280, 13), // "btn_e_clicked"
QT_MOC_LITERAL(20, 294, 13), // "btn_f_clicked"
QT_MOC_LITERAL(21, 308, 16), // "btn_left_clicked"
QT_MOC_LITERAL(22, 325, 17), // "btn_right_clicked"
QT_MOC_LITERAL(23, 343, 17) // "btn_clear_clicked"

    },
    "MyInput\0ensureSetVal\0\0btn_one_clicked\0"
    "btn_two_clicked\0btn_three_clicked\0"
    "btn_four_clicked\0btn_five_clicked\0"
    "btn_six_clicked\0btn_eight_clicked\0"
    "btn_seven_clicked\0btn_nine_clicked\0"
    "btn_zero_clicked\0btn_dec_clicked\0"
    "btn_neg_clicked\0btn_a_clicked\0"
    "btn_b_clicked\0btn_c_clicked\0btn_d_clicked\0"
    "btn_e_clicked\0btn_f_clicked\0"
    "btn_left_clicked\0btn_right_clicked\0"
    "btn_clear_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyInput[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x0a /* Public */,
       3,    0,  125,    2, 0x08 /* Private */,
       4,    0,  126,    2, 0x08 /* Private */,
       5,    0,  127,    2, 0x08 /* Private */,
       6,    0,  128,    2, 0x08 /* Private */,
       7,    0,  129,    2, 0x08 /* Private */,
       8,    0,  130,    2, 0x08 /* Private */,
       9,    0,  131,    2, 0x08 /* Private */,
      10,    0,  132,    2, 0x08 /* Private */,
      11,    0,  133,    2, 0x08 /* Private */,
      12,    0,  134,    2, 0x08 /* Private */,
      13,    0,  135,    2, 0x08 /* Private */,
      14,    0,  136,    2, 0x08 /* Private */,
      15,    0,  137,    2, 0x08 /* Private */,
      16,    0,  138,    2, 0x08 /* Private */,
      17,    0,  139,    2, 0x08 /* Private */,
      18,    0,  140,    2, 0x08 /* Private */,
      19,    0,  141,    2, 0x08 /* Private */,
      20,    0,  142,    2, 0x08 /* Private */,
      21,    0,  143,    2, 0x08 /* Private */,
      22,    0,  144,    2, 0x08 /* Private */,
      23,    0,  145,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyInput *_t = static_cast<MyInput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensureSetVal(); break;
        case 1: _t->btn_one_clicked(); break;
        case 2: _t->btn_two_clicked(); break;
        case 3: _t->btn_three_clicked(); break;
        case 4: _t->btn_four_clicked(); break;
        case 5: _t->btn_five_clicked(); break;
        case 6: _t->btn_six_clicked(); break;
        case 7: _t->btn_eight_clicked(); break;
        case 8: _t->btn_seven_clicked(); break;
        case 9: _t->btn_nine_clicked(); break;
        case 10: _t->btn_zero_clicked(); break;
        case 11: _t->btn_dec_clicked(); break;
        case 12: _t->btn_neg_clicked(); break;
        case 13: _t->btn_a_clicked(); break;
        case 14: _t->btn_b_clicked(); break;
        case 15: _t->btn_c_clicked(); break;
        case 16: _t->btn_d_clicked(); break;
        case 17: _t->btn_e_clicked(); break;
        case 18: _t->btn_f_clicked(); break;
        case 19: _t->btn_left_clicked(); break;
        case 20: _t->btn_right_clicked(); break;
        case 21: _t->btn_clear_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MyInput::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MyInput.data,
      qt_meta_data_MyInput,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyInput::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyInput.stringdata0))
        return static_cast<void*>(const_cast< MyInput*>(this));
    return QDialog::qt_metacast(_clname);
}

int MyInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}
struct qt_meta_stringdata_ColorCombox_t {
    QByteArrayData data[6];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ColorCombox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ColorCombox_t qt_meta_stringdata_ColorCombox = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ColorCombox"
QT_MOC_LITERAL(1, 12, 15), // "sigColorChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 5), // "color"
QT_MOC_LITERAL(4, 35, 13), // "setButtonIcon"
QT_MOC_LITERAL(5, 49, 16) // "OnShowColorBoard"

    },
    "ColorCombox\0sigColorChanged\0\0color\0"
    "setButtonIcon\0OnShowColorBoard"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColorCombox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x0a /* Public */,
       5,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void,

       0        // eod
};

void ColorCombox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColorCombox *_t = static_cast<ColorCombox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigColorChanged((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: _t->setButtonIcon((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->OnShowColorBoard(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ColorCombox::*_t)(QColor );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColorCombox::sigColorChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ColorCombox::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_ColorCombox.data,
      qt_meta_data_ColorCombox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ColorCombox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorCombox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ColorCombox.stringdata0))
        return static_cast<void*>(const_cast< ColorCombox*>(this));
    return QToolButton::qt_metacast(_clname);
}

int ColorCombox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ColorCombox::sigColorChanged(QColor _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_myTabBar_t {
    QByteArrayData data[5];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myTabBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myTabBar_t qt_meta_stringdata_myTabBar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "myTabBar"
QT_MOC_LITERAL(1, 9, 11), // "starDragTab"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 5), // "index"
QT_MOC_LITERAL(4, 28, 10) // "endDragTab"

    },
    "myTabBar\0starDragTab\0\0index\0endDragTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myTabBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

       0        // eod
};

void myTabBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myTabBar *_t = static_cast<myTabBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->starDragTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->endDragTab(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (myTabBar::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&myTabBar::starDragTab)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (myTabBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&myTabBar::endDragTab)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject myTabBar::staticMetaObject = {
    { &QTabBar::staticMetaObject, qt_meta_stringdata_myTabBar.data,
      qt_meta_data_myTabBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *myTabBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myTabBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_myTabBar.stringdata0))
        return static_cast<void*>(const_cast< myTabBar*>(this));
    return QTabBar::qt_metacast(_clname);
}

int myTabBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void myTabBar::starDragTab(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void myTabBar::endDragTab()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_CMyTabWidget_t {
    QByteArrayData data[5];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMyTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMyTabWidget_t qt_meta_stringdata_CMyTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CMyTabWidget"
QT_MOC_LITERAL(1, 13, 8), // "starDrag"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "index"
QT_MOC_LITERAL(4, 29, 7) // "endDrag"

    },
    "CMyTabWidget\0starDrag\0\0index\0endDrag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMyTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       4,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

       0        // eod
};

void CMyTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMyTabWidget *_t = static_cast<CMyTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->starDrag((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->endDrag(); break;
        default: ;
        }
    }
}

const QMetaObject CMyTabWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_CMyTabWidget.data,
      qt_meta_data_CMyTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CMyTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMyTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CMyTabWidget.stringdata0))
        return static_cast<void*>(const_cast< CMyTabWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int CMyTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_MyGetValue_t {
    QByteArrayData data[6];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyGetValue_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyGetValue_t qt_meta_stringdata_MyGetValue = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MyGetValue"
QT_MOC_LITERAL(1, 11, 10), // "valueClick"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "getValue"
QT_MOC_LITERAL(4, 32, 8), // "setValue"
QT_MOC_LITERAL(5, 41, 5) // "_text"

    },
    "MyGetValue\0valueClick\0\0getValue\0"
    "setValue\0_text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyGetValue[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x0a /* Public */,
       4,    1,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void MyGetValue::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyGetValue *_t = static_cast<MyGetValue *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueClick(); break;
        case 1: { QString _r = _t->getValue();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: _t->setValue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyGetValue::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyGetValue::valueClick)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyGetValue::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MyGetValue.data,
      qt_meta_data_MyGetValue,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyGetValue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGetValue::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyGetValue.stringdata0))
        return static_cast<void*>(const_cast< MyGetValue*>(this));
    return QWidget::qt_metacast(_clname);
}

int MyGetValue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MyGetValue::valueClick()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_MyPushButton_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyPushButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyPushButton_t qt_meta_stringdata_MyPushButton = {
    {
QT_MOC_LITERAL(0, 0, 12) // "MyPushButton"

    },
    "MyPushButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyPushButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MyPushButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MyPushButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_MyPushButton.data,
      qt_meta_data_MyPushButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyPushButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyPushButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyPushButton.stringdata0))
        return static_cast<void*>(const_cast< MyPushButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int MyPushButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_IOButton_t {
    QByteArrayData data[3];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IOButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IOButton_t qt_meta_stringdata_IOButton = {
    {
QT_MOC_LITERAL(0, 0, 8), // "IOButton"
QT_MOC_LITERAL(1, 9, 9), // "btnPresse"
QT_MOC_LITERAL(2, 19, 0) // ""

    },
    "IOButton\0btnPresse\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IOButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void IOButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IOButton *_t = static_cast<IOButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btnPresse(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject IOButton::staticMetaObject = {
    { &MyPushButton::staticMetaObject, qt_meta_stringdata_IOButton.data,
      qt_meta_data_IOButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IOButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IOButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IOButton.stringdata0))
        return static_cast<void*>(const_cast< IOButton*>(this));
    return MyPushButton::qt_metacast(_clname);
}

int IOButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MyPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MylabelVar_t {
    QByteArrayData data[7];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MylabelVar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MylabelVar_t qt_meta_stringdata_MylabelVar = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MylabelVar"
QT_MOC_LITERAL(1, 11, 11), // "changeValue"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 9), // "isChanged"
QT_MOC_LITERAL(4, 34, 11), // "MylabelVar*"
QT_MOC_LITERAL(5, 46, 14), // "valueisChanged"
QT_MOC_LITERAL(6, 61, 7) // "clicked"

    },
    "MylabelVar\0changeValue\0\0isChanged\0"
    "MylabelVar*\0valueisChanged\0clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MylabelVar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    1,   37,    2, 0x06 /* Public */,
       5,    1,   40,    2, 0x06 /* Public */,
       6,    0,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,

       0        // eod
};

void MylabelVar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MylabelVar *_t = static_cast<MylabelVar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->isChanged((*reinterpret_cast< MylabelVar*(*)>(_a[1]))); break;
        case 2: _t->valueisChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MylabelVar* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MylabelVar::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MylabelVar::changeValue)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MylabelVar::*_t)(MylabelVar * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MylabelVar::isChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MylabelVar::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MylabelVar::valueisChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MylabelVar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MylabelVar::clicked)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject MylabelVar::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MylabelVar.data,
      qt_meta_data_MylabelVar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MylabelVar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MylabelVar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MylabelVar.stringdata0))
        return static_cast<void*>(const_cast< MylabelVar*>(this));
    return QLabel::qt_metacast(_clname);
}

int MylabelVar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MylabelVar::changeValue(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MylabelVar::isChanged(MylabelVar * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MylabelVar::valueisChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MylabelVar::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
struct qt_meta_stringdata_Mybtn_t {
    QByteArrayData data[3];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Mybtn_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Mybtn_t qt_meta_stringdata_Mybtn = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Mybtn"
QT_MOC_LITERAL(1, 6, 11), // "btn_release"
QT_MOC_LITERAL(2, 18, 0) // ""

    },
    "Mybtn\0btn_release\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mybtn[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Mybtn::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Mybtn *_t = static_cast<Mybtn *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btn_release(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Mybtn::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_Mybtn.data,
      qt_meta_data_Mybtn,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Mybtn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mybtn::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Mybtn.stringdata0))
        return static_cast<void*>(const_cast< Mybtn*>(this));
    return QPushButton::qt_metacast(_clname);
}

int Mybtn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MyRstbtn_t {
    QByteArrayData data[3];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyRstbtn_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyRstbtn_t qt_meta_stringdata_MyRstbtn = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyRstbtn"
QT_MOC_LITERAL(1, 9, 11), // "btn_release"
QT_MOC_LITERAL(2, 21, 0) // ""

    },
    "MyRstbtn\0btn_release\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyRstbtn[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MyRstbtn::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyRstbtn *_t = static_cast<MyRstbtn *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btn_release(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MyRstbtn::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_MyRstbtn.data,
      qt_meta_data_MyRstbtn,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyRstbtn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyRstbtn::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyRstbtn.stringdata0))
        return static_cast<void*>(const_cast< MyRstbtn*>(this));
    return QPushButton::qt_metacast(_clname);
}

int MyRstbtn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MyProgamBtn_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyProgamBtn_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyProgamBtn_t qt_meta_stringdata_MyProgamBtn = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MyProgamBtn"
QT_MOC_LITERAL(1, 12, 9), // "btn_click"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "MyProgamBtn\0btn_click\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyProgamBtn[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MyProgamBtn::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyProgamBtn *_t = static_cast<MyProgamBtn *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btn_click(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MyProgamBtn::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_MyProgamBtn.data,
      qt_meta_data_MyProgamBtn,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyProgamBtn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyProgamBtn::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyProgamBtn.stringdata0))
        return static_cast<void*>(const_cast< MyProgamBtn*>(this));
    return QPushButton::qt_metacast(_clname);
}

int MyProgamBtn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MySetVar_t {
    QByteArrayData data[5];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MySetVar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MySetVar_t qt_meta_stringdata_MySetVar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MySetVar"
QT_MOC_LITERAL(1, 9, 7), // "clicked"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 11), // "changeValue"
QT_MOC_LITERAL(4, 30, 14) // "valueisChanged"

    },
    "MySetVar\0clicked\0\0changeValue\0"
    "valueisChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MySetVar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,
       4,    1,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void MySetVar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MySetVar *_t = static_cast<MySetVar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->changeValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->valueisChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MySetVar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetVar::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MySetVar::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetVar::changeValue)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MySetVar::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetVar::valueisChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject MySetVar::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MySetVar.data,
      qt_meta_data_MySetVar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MySetVar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySetVar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MySetVar.stringdata0))
        return static_cast<void*>(const_cast< MySetVar*>(this));
    return QLabel::qt_metacast(_clname);
}

int MySetVar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MySetVar::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MySetVar::changeValue(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MySetVar::valueisChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_CAutoMessageBox_t {
    QByteArrayData data[4];
    char stringdata0[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CAutoMessageBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CAutoMessageBox_t qt_meta_stringdata_CAutoMessageBox = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CAutoMessageBox"
QT_MOC_LITERAL(1, 16, 12), // "btn_yes_slot"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11) // "btn_no_slot"

    },
    "CAutoMessageBox\0btn_yes_slot\0\0btn_no_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CAutoMessageBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CAutoMessageBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAutoMessageBox *_t = static_cast<CAutoMessageBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btn_yes_slot(); break;
        case 1: _t->btn_no_slot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CAutoMessageBox::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CAutoMessageBox.data,
      qt_meta_data_CAutoMessageBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CAutoMessageBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CAutoMessageBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CAutoMessageBox.stringdata0))
        return static_cast<void*>(const_cast< CAutoMessageBox*>(this));
    return QDialog::qt_metacast(_clname);
}

int CAutoMessageBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_CAutoMessageBox_2_t {
    QByteArrayData data[6];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CAutoMessageBox_2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CAutoMessageBox_2_t qt_meta_stringdata_CAutoMessageBox_2 = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CAutoMessageBox_2"
QT_MOC_LITERAL(1, 18, 12), // "btn_run_slot"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 17), // "btn_breakRun_slot"
QT_MOC_LITERAL(4, 50, 14), // "btn_myRun_slot"
QT_MOC_LITERAL(5, 65, 3) // "run"

    },
    "CAutoMessageBox_2\0btn_run_slot\0\0"
    "btn_breakRun_slot\0btn_myRun_slot\0run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CAutoMessageBox_2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CAutoMessageBox_2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAutoMessageBox_2 *_t = static_cast<CAutoMessageBox_2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btn_run_slot(); break;
        case 1: _t->btn_breakRun_slot(); break;
        case 2: _t->btn_myRun_slot(); break;
        case 3: _t->run(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CAutoMessageBox_2::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CAutoMessageBox_2.data,
      qt_meta_data_CAutoMessageBox_2,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CAutoMessageBox_2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CAutoMessageBox_2::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CAutoMessageBox_2.stringdata0))
        return static_cast<void*>(const_cast< CAutoMessageBox_2*>(this));
    return QDialog::qt_metacast(_clname);
}

int CAutoMessageBox_2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_MySetTime_t {
    QByteArrayData data[9];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MySetTime_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MySetTime_t qt_meta_stringdata_MySetTime = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MySetTime"
QT_MOC_LITERAL(1, 10, 10), // "timeChange"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "setHour"
QT_MOC_LITERAL(4, 30, 9), // "setMinute"
QT_MOC_LITERAL(5, 40, 6), // "setSec"
QT_MOC_LITERAL(6, 47, 7), // "setTime"
QT_MOC_LITERAL(7, 55, 4), // "_sec"
QT_MOC_LITERAL(8, 60, 7) // "getTime"

    },
    "MySetTime\0timeChange\0\0setHour\0setMinute\0"
    "setSec\0setTime\0_sec\0getTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MySetTime[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   47,    2, 0x0a /* Public */,
       4,    0,   48,    2, 0x0a /* Public */,
       5,    0,   49,    2, 0x0a /* Public */,
       6,    1,   50,    2, 0x0a /* Public */,
       8,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Int, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, QMetaType::Int,    7,
    QMetaType::Int,

       0        // eod
};

void MySetTime::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MySetTime *_t = static_cast<MySetTime *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->timeChange((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: _t->setHour(); break;
        case 2: _t->setMinute(); break;
        case 3: _t->setSec(); break;
        case 4: { int _r = _t->setTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->getTime();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef int (MySetTime::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySetTime::timeChange)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MySetTime::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MySetTime.data,
      qt_meta_data_MySetTime,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MySetTime::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySetTime::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MySetTime.stringdata0))
        return static_cast<void*>(const_cast< MySetTime*>(this));
    return QWidget::qt_metacast(_clname);
}

int MySetTime::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
int MySetTime::timeChange(int _t1)
{
    int _t0 = int();
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
struct qt_meta_stringdata_DecKeypad_t {
    QByteArrayData data[5];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DecKeypad_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DecKeypad_t qt_meta_stringdata_DecKeypad = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DecKeypad"
QT_MOC_LITERAL(1, 10, 12), // "ensureSetVal"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "get_btn_sig"
QT_MOC_LITERAL(4, 36, 12) // "get_btn_sig2"

    },
    "DecKeypad\0ensureSetVal\0\0get_btn_sig\0"
    "get_btn_sig2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DecKeypad[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DecKeypad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DecKeypad *_t = static_cast<DecKeypad *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensureSetVal(); break;
        case 1: _t->get_btn_sig(); break;
        case 2: _t->get_btn_sig2(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject DecKeypad::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DecKeypad.data,
      qt_meta_data_DecKeypad,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DecKeypad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DecKeypad::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DecKeypad.stringdata0))
        return static_cast<void*>(const_cast< DecKeypad*>(this));
    return QDialog::qt_metacast(_clname);
}

int DecKeypad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_MyRadio_t {
    QByteArrayData data[4];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyRadio_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyRadio_t qt_meta_stringdata_MyRadio = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MyRadio"
QT_MOC_LITERAL(1, 8, 12), // "ensureSetVal"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 14) // "encancelSetVal"

    },
    "MyRadio\0ensureSetVal\0\0encancelSetVal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyRadio[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyRadio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyRadio *_t = static_cast<MyRadio *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ensureSetVal(); break;
        case 1: _t->encancelSetVal(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MyRadio::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MyRadio.data,
      qt_meta_data_MyRadio,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyRadio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyRadio::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyRadio.stringdata0))
        return static_cast<void*>(const_cast< MyRadio*>(this));
    return QDialog::qt_metacast(_clname);
}

int MyRadio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
