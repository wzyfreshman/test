/****************************************************************************
** Meta object code from reading C++ file 'mainframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frame/mainframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainFrame_t {
    QByteArrayData data[30];
    char stringdata0[327];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainFrame_t qt_meta_stringdata_MainFrame = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MainFrame"
QT_MOC_LITERAL(1, 10, 7), // "operate"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 8), // "showFull"
QT_MOC_LITERAL(4, 28, 6), // "clrMsg"
QT_MOC_LITERAL(5, 35, 7), // "sendCMD"
QT_MOC_LITERAL(6, 43, 8), // "sendJson"
QT_MOC_LITERAL(7, 52, 7), // "showMsg"
QT_MOC_LITERAL(8, 60, 16), // "sendMessageToCAM"
QT_MOC_LITERAL(9, 77, 14), // "setCamEditMode"
QT_MOC_LITERAL(10, 92, 6), // "isEdit"
QT_MOC_LITERAL(11, 99, 10), // "CMDFromCam"
QT_MOC_LITERAL(12, 110, 3), // "cmd"
QT_MOC_LITERAL(13, 114, 14), // "messageFromCam"
QT_MOC_LITERAL(14, 129, 4), // "type"
QT_MOC_LITERAL(15, 134, 3), // "msg"
QT_MOC_LITERAL(16, 138, 11), // "getPanelNew"
QT_MOC_LITERAL(17, 150, 10), // "switchPage"
QT_MOC_LITERAL(18, 161, 5), // "index"
QT_MOC_LITERAL(19, 167, 15), // "updateCamWidget"
QT_MOC_LITERAL(20, 183, 13), // "programAborts"
QT_MOC_LITERAL(21, 197, 13), // "check_alm_sig"
QT_MOC_LITERAL(22, 211, 15), // "sendHightSignal"
QT_MOC_LITERAL(23, 227, 13), // "sendLowSignal"
QT_MOC_LITERAL(24, 241, 17), // "sendHightSignalv2"
QT_MOC_LITERAL(25, 259, 15), // "sendLowSignalv2"
QT_MOC_LITERAL(26, 275, 16), // "sendSwitchChange"
QT_MOC_LITERAL(27, 292, 15), // "sendExternEstop"
QT_MOC_LITERAL(28, 308, 3), // "flg"
QT_MOC_LITERAL(29, 312, 14) // "get_move_order"

    },
    "MainFrame\0operate\0\0showFull\0clrMsg\0"
    "sendCMD\0sendJson\0showMsg\0sendMessageToCAM\0"
    "setCamEditMode\0isEdit\0CMDFromCam\0cmd\0"
    "messageFromCam\0type\0msg\0getPanelNew\0"
    "switchPage\0index\0updateCamWidget\0"
    "programAborts\0check_alm_sig\0sendHightSignal\0"
    "sendLowSignal\0sendHightSignalv2\0"
    "sendLowSignalv2\0sendSwitchChange\0"
    "sendExternEstop\0flg\0get_move_order"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x06 /* Public */,
       3,    1,  125,    2, 0x06 /* Public */,
       4,    0,  128,    2, 0x06 /* Public */,
       5,    1,  129,    2, 0x06 /* Public */,
       6,    1,  132,    2, 0x06 /* Public */,
       7,    2,  135,    2, 0x06 /* Public */,
       8,    1,  140,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,  143,    2, 0x0a /* Public */,
      11,    1,  146,    2, 0x0a /* Public */,
      13,    2,  149,    2, 0x0a /* Public */,
      16,    0,  154,    2, 0x0a /* Public */,
      17,    1,  155,    2, 0x08 /* Private */,
      19,    0,  158,    2, 0x08 /* Private */,
      20,    0,  159,    2, 0x08 /* Private */,
      21,    0,  160,    2, 0x08 /* Private */,
      22,    1,  161,    2, 0x08 /* Private */,
      23,    1,  164,    2, 0x08 /* Private */,
      24,    1,  167,    2, 0x08 /* Private */,
      25,    1,  170,    2, 0x08 /* Private */,
      26,    0,  173,    2, 0x08 /* Private */,
      27,    1,  174,    2, 0x08 /* Private */,
      29,    0,  177,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QJsonObject,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void,

       0        // eod
};

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainFrame *_t = static_cast<MainFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->operate(); break;
        case 1: _t->showFull((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->clrMsg(); break;
        case 3: _t->sendCMD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sendJson((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 5: _t->showMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->sendMessageToCAM((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setCamEditMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->CMDFromCam((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->messageFromCam((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->getPanelNew(); break;
        case 11: _t->switchPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->updateCamWidget(); break;
        case 13: _t->programAborts(); break;
        case 14: _t->check_alm_sig(); break;
        case 15: _t->sendHightSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->sendLowSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->sendHightSignalv2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->sendLowSignalv2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->sendSwitchChange(); break;
        case 20: _t->sendExternEstop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->get_move_order(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::operate)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainFrame::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::showFull)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::clrMsg)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainFrame::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::sendCMD)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MainFrame::*_t)(QJsonObject );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::sendJson)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MainFrame::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::showMsg)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MainFrame::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainFrame::sendMessageToCAM)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject MainFrame::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainFrame.data,
      qt_meta_data_MainFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrame.stringdata0))
        return static_cast<void*>(const_cast< MainFrame*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void MainFrame::operate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MainFrame::showFull(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainFrame::clrMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void MainFrame::sendCMD(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainFrame::sendJson(QJsonObject _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainFrame::showMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainFrame::sendMessageToCAM(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
