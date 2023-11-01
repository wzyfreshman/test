/****************************************************************************
** Meta object code from reading C++ file 'gl_viewwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gl/gl_viewwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gl_viewwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GL_PostionLogger_t {
    QByteArrayData data[12];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GL_PostionLogger_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GL_PostionLogger_t qt_meta_stringdata_GL_PostionLogger = {
    {
QT_MOC_LITERAL(0, 0, 16), // "GL_PostionLogger"
QT_MOC_LITERAL(1, 17, 11), // "resultReady"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 6), // "result"
QT_MOC_LITERAL(4, 37, 8), // "doLogger"
QT_MOC_LITERAL(5, 46, 10), // "loggerCall"
QT_MOC_LITERAL(6, 57, 5), // "clear"
QT_MOC_LITERAL(7, 63, 12), // "lastPosition"
QT_MOC_LITERAL(8, 76, 13), // "logger_point*"
QT_MOC_LITERAL(9, 90, 14), // "setLoggerState"
QT_MOC_LITERAL(10, 105, 24), // "PositionLoggerContainer*"
QT_MOC_LITERAL(11, 130, 1) // "s"

    },
    "GL_PostionLogger\0resultReady\0\0result\0"
    "doLogger\0loggerCall\0clear\0lastPosition\0"
    "logger_point*\0setLoggerState\0"
    "PositionLoggerContainer*\0s"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GL_PostionLogger[] = {

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
       4,    0,   47,    2, 0x0a /* Public */,
       5,    0,   48,    2, 0x0a /* Public */,
       6,    0,   49,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x0a /* Public */,
       9,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 8,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void GL_PostionLogger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GL_PostionLogger *_t = static_cast<GL_PostionLogger *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resultReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->doLogger(); break;
        case 2: _t->loggerCall(); break;
        case 3: _t->clear(); break;
        case 4: { logger_point* _r = _t->lastPosition();
            if (_a[0]) *reinterpret_cast< logger_point**>(_a[0]) = _r; }  break;
        case 5: _t->setLoggerState((*reinterpret_cast< PositionLoggerContainer*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GL_PostionLogger::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_PostionLogger::resultReady)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GL_PostionLogger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GL_PostionLogger.data,
      qt_meta_data_GL_PostionLogger,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GL_PostionLogger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GL_PostionLogger::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GL_PostionLogger.stringdata0))
        return static_cast<void*>(const_cast< GL_PostionLogger*>(this));
    return QObject::qt_metacast(_clname);
}

int GL_PostionLogger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void GL_PostionLogger::resultReady(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_GL_ViewWidget_t {
    QByteArrayData data[28];
    char stringdata0[315];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GL_ViewWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GL_ViewWidget_t qt_meta_stringdata_GL_ViewWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GL_ViewWidget"
QT_MOC_LITERAL(1, 14, 19), // "signalSetLoadingMax"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5), // "value"
QT_MOC_LITERAL(4, 41, 21), // "signalSetLoadingValue"
QT_MOC_LITERAL(5, 63, 15), // "signalErrorInfo"
QT_MOC_LITERAL(6, 79, 20), // "signalSelectHighLine"
QT_MOC_LITERAL(7, 100, 17), // "signalStartThread"
QT_MOC_LITERAL(8, 118, 5), // "viewX"
QT_MOC_LITERAL(9, 124, 5), // "viewY"
QT_MOC_LITERAL(10, 130, 5), // "viewZ"
QT_MOC_LITERAL(11, 136, 5), // "viewP"
QT_MOC_LITERAL(12, 142, 6), // "viewZ2"
QT_MOC_LITERAL(13, 149, 5), // "clear"
QT_MOC_LITERAL(14, 155, 6), // "reload"
QT_MOC_LITERAL(15, 162, 6), // "zoomIn"
QT_MOC_LITERAL(16, 169, 7), // "zoomOut"
QT_MOC_LITERAL(17, 177, 11), // "setHighLine"
QT_MOC_LITERAL(18, 189, 7), // "linenum"
QT_MOC_LITERAL(19, 197, 10), // "clearModel"
QT_MOC_LITERAL(20, 208, 9), // "abortLoad"
QT_MOC_LITERAL(21, 218, 13), // "setNcFileName"
QT_MOC_LITERAL(22, 232, 4), // "name"
QT_MOC_LITERAL(23, 237, 21), // "parsingFileLineNumber"
QT_MOC_LITERAL(24, 259, 23), // "setParsingFileLineCount"
QT_MOC_LITERAL(25, 283, 16), // "parsingErrorInfo"
QT_MOC_LITERAL(26, 300, 3), // "str"
QT_MOC_LITERAL(27, 304, 10) // "debugPrint"

    },
    "GL_ViewWidget\0signalSetLoadingMax\0\0"
    "value\0signalSetLoadingValue\0signalErrorInfo\0"
    "signalSelectHighLine\0signalStartThread\0"
    "viewX\0viewY\0viewZ\0viewP\0viewZ2\0clear\0"
    "reload\0zoomIn\0zoomOut\0setHighLine\0"
    "linenum\0clearModel\0abortLoad\0setNcFileName\0"
    "name\0parsingFileLineNumber\0"
    "setParsingFileLineCount\0parsingErrorInfo\0"
    "str\0debugPrint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GL_ViewWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x06 /* Public */,
       4,    1,  127,    2, 0x06 /* Public */,
       5,    2,  130,    2, 0x06 /* Public */,
       6,    1,  135,    2, 0x06 /* Public */,
       7,    0,  138,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  139,    2, 0x0a /* Public */,
       9,    0,  140,    2, 0x0a /* Public */,
      10,    0,  141,    2, 0x0a /* Public */,
      11,    0,  142,    2, 0x0a /* Public */,
      12,    0,  143,    2, 0x0a /* Public */,
      13,    0,  144,    2, 0x0a /* Public */,
      14,    0,  145,    2, 0x0a /* Public */,
      15,    0,  146,    2, 0x0a /* Public */,
      16,    0,  147,    2, 0x0a /* Public */,
      17,    1,  148,    2, 0x0a /* Public */,
      19,    0,  151,    2, 0x0a /* Public */,
      20,    0,  152,    2, 0x0a /* Public */,
      21,    1,  153,    2, 0x0a /* Public */,
      23,    1,  156,    2, 0x0a /* Public */,
      24,    1,  159,    2, 0x0a /* Public */,
      25,    1,  162,    2, 0x0a /* Public */,
      27,    1,  165,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

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
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void, QMetaType::QString,   26,

       0        // eod
};

void GL_ViewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GL_ViewWidget *_t = static_cast<GL_ViewWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSetLoadingMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signalSetLoadingValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signalErrorInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->signalSelectHighLine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->signalStartThread(); break;
        case 5: _t->viewX(); break;
        case 6: _t->viewY(); break;
        case 7: _t->viewZ(); break;
        case 8: _t->viewP(); break;
        case 9: _t->viewZ2(); break;
        case 10: _t->clear(); break;
        case 11: _t->reload(); break;
        case 12: _t->zoomIn(); break;
        case 13: _t->zoomOut(); break;
        case 14: _t->setHighLine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->clearModel(); break;
        case 16: _t->abortLoad(); break;
        case 17: _t->setNcFileName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->parsingFileLineNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->setParsingFileLineCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->parsingErrorInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->debugPrint((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GL_ViewWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_ViewWidget::signalSetLoadingMax)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GL_ViewWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_ViewWidget::signalSetLoadingValue)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (GL_ViewWidget::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_ViewWidget::signalErrorInfo)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (GL_ViewWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_ViewWidget::signalSelectHighLine)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (GL_ViewWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GL_ViewWidget::signalStartThread)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject GL_ViewWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GL_ViewWidget.data,
      qt_meta_data_GL_ViewWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GL_ViewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GL_ViewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GL_ViewWidget.stringdata0))
        return static_cast<void*>(const_cast< GL_ViewWidget*>(this));
    if (!strcmp(_clname, "GL_ViewDraw"))
        return static_cast< GL_ViewDraw*>(const_cast< GL_ViewWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GL_ViewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void GL_ViewWidget::signalSetLoadingMax(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GL_ViewWidget::signalSetLoadingValue(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GL_ViewWidget::signalErrorInfo(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GL_ViewWidget::signalSelectHighLine(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GL_ViewWidget::signalStartThread()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
