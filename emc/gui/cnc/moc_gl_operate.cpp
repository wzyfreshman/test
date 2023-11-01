/****************************************************************************
** Meta object code from reading C++ file 'gl_operate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gl/gl_operate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gl_operate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GL_Operate_t {
    QByteArrayData data[7];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GL_Operate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GL_Operate_t qt_meta_stringdata_GL_Operate = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GL_Operate"
QT_MOC_LITERAL(1, 11, 10), // "loadNcfile"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "file"
QT_MOC_LITERAL(4, 28, 12), // "colorSetting"
QT_MOC_LITERAL(5, 41, 8), // "loadFile"
QT_MOC_LITERAL(6, 50, 9) // "reflushGL"

    },
    "GL_Operate\0loadNcfile\0\0file\0colorSetting\0"
    "loadFile\0reflushGL"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GL_Operate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   37,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x0a /* Public */,
       6,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void GL_Operate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GL_Operate *_t = static_cast<GL_Operate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadNcfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->colorSetting(); break;
        case 2: _t->loadFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->reflushGL(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GL_Operate::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_Operate::loadNcfile)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GL_Operate::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GL_Operate.data,
      qt_meta_data_GL_Operate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GL_Operate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GL_Operate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GL_Operate.stringdata0))
        return static_cast<void*>(const_cast< GL_Operate*>(this));
    return QWidget::qt_metacast(_clname);
}

int GL_Operate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void GL_Operate::loadNcfile(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
