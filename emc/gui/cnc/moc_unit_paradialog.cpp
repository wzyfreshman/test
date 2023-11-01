/****************************************************************************
** Meta object code from reading C++ file 'unit_paradialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/unit_paradialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unit_paradialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GroupTableWidget_t {
    QByteArrayData data[7];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GroupTableWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GroupTableWidget_t qt_meta_stringdata_GroupTableWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "GroupTableWidget"
QT_MOC_LITERAL(1, 17, 10), // "rowEntered"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "row"
QT_MOC_LITERAL(4, 33, 6), // "column"
QT_MOC_LITERAL(5, 40, 12), // "rowDbClicked"
QT_MOC_LITERAL(6, 53, 13) // "mycellEntered"

    },
    "GroupTableWidget\0rowEntered\0\0row\0"
    "column\0rowDbClicked\0mycellEntered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GroupTableWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       5,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

       0        // eod
};

void GroupTableWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GroupTableWidget *_t = static_cast<GroupTableWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rowEntered((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->rowDbClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->mycellEntered((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GroupTableWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GroupTableWidget::rowEntered)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GroupTableWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GroupTableWidget::rowDbClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject GroupTableWidget::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_GroupTableWidget.data,
      qt_meta_data_GroupTableWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GroupTableWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GroupTableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GroupTableWidget.stringdata0))
        return static_cast<void*>(const_cast< GroupTableWidget*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int GroupTableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
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
void GroupTableWidget::rowEntered(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GroupTableWidget::rowDbClicked(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_Par_Dialog_t {
    QByteArrayData data[37];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Par_Dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Par_Dialog_t qt_meta_stringdata_Par_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Par_Dialog"
QT_MOC_LITERAL(1, 11, 8), // "send_msg"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "str"
QT_MOC_LITERAL(4, 25, 12), // "table_change"
QT_MOC_LITERAL(5, 38, 1), // "x"
QT_MOC_LITERAL(6, 40, 1), // "y"
QT_MOC_LITERAL(7, 42, 4), // "type"
QT_MOC_LITERAL(8, 47, 10), // "par_cancel"
QT_MOC_LITERAL(9, 58, 11), // "show_detail"
QT_MOC_LITERAL(10, 70, 18), // "switch_to_operator"
QT_MOC_LITERAL(11, 89, 5), // "value"
QT_MOC_LITERAL(12, 95, 15), // "switch_to_debug"
QT_MOC_LITERAL(13, 111, 17), // "switch_to_develop"
QT_MOC_LITERAL(14, 129, 13), // "getcellselect"
QT_MOC_LITERAL(15, 143, 18), // "getuserDataChanged"
QT_MOC_LITERAL(16, 162, 10), // "Key_Return"
QT_MOC_LITERAL(17, 173, 5), // "Key_O"
QT_MOC_LITERAL(18, 179, 5), // "Key_M"
QT_MOC_LITERAL(19, 185, 5), // "Key_D"
QT_MOC_LITERAL(20, 191, 5), // "Key_P"
QT_MOC_LITERAL(21, 197, 5), // "Key_F"
QT_MOC_LITERAL(22, 203, 5), // "Key_I"
QT_MOC_LITERAL(23, 209, 5), // "Key_H"
QT_MOC_LITERAL(24, 215, 5), // "Key_R"
QT_MOC_LITERAL(25, 221, 5), // "Key_V"
QT_MOC_LITERAL(26, 227, 5), // "Key_T"
QT_MOC_LITERAL(27, 233, 5), // "Key_Q"
QT_MOC_LITERAL(28, 239, 5), // "Key_S"
QT_MOC_LITERAL(29, 245, 13), // "getFindselect"
QT_MOC_LITERAL(30, 259, 3), // "row"
QT_MOC_LITERAL(31, 263, 6), // "column"
QT_MOC_LITERAL(32, 270, 8), // "ParaFind"
QT_MOC_LITERAL(33, 279, 13), // "slot_MenuGoto"
QT_MOC_LITERAL(34, 293, 3), // "pos"
QT_MOC_LITERAL(35, 297, 12), // "slot_TblGoto"
QT_MOC_LITERAL(36, 310, 10) // "tabChanged"

    },
    "Par_Dialog\0send_msg\0\0str\0table_change\0"
    "x\0y\0type\0par_cancel\0show_detail\0"
    "switch_to_operator\0value\0switch_to_debug\0"
    "switch_to_develop\0getcellselect\0"
    "getuserDataChanged\0Key_Return\0Key_O\0"
    "Key_M\0Key_D\0Key_P\0Key_F\0Key_I\0Key_H\0"
    "Key_R\0Key_V\0Key_T\0Key_Q\0Key_S\0"
    "getFindselect\0row\0column\0ParaFind\0"
    "slot_MenuGoto\0pos\0slot_TblGoto\0"
    "tabChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Par_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  154,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    3,  159,    2, 0x0a /* Public */,
       8,    0,  166,    2, 0x0a /* Public */,
       9,    2,  167,    2, 0x0a /* Public */,
       9,    4,  172,    2, 0x0a /* Public */,
      10,    1,  181,    2, 0x0a /* Public */,
      12,    1,  184,    2, 0x0a /* Public */,
      13,    1,  187,    2, 0x0a /* Public */,
      14,    2,  190,    2, 0x0a /* Public */,
      15,    2,  195,    2, 0x0a /* Public */,
      16,    0,  200,    2, 0x0a /* Public */,
      17,    0,  201,    2, 0x0a /* Public */,
      18,    0,  202,    2, 0x0a /* Public */,
      19,    0,  203,    2, 0x0a /* Public */,
      20,    0,  204,    2, 0x0a /* Public */,
      21,    0,  205,    2, 0x0a /* Public */,
      22,    0,  206,    2, 0x0a /* Public */,
      23,    0,  207,    2, 0x0a /* Public */,
      24,    0,  208,    2, 0x0a /* Public */,
      25,    0,  209,    2, 0x0a /* Public */,
      26,    0,  210,    2, 0x0a /* Public */,
      27,    0,  211,    2, 0x0a /* Public */,
      28,    0,  212,    2, 0x0a /* Public */,
      29,    2,  213,    2, 0x0a /* Public */,
      32,    0,  218,    2, 0x0a /* Public */,
      33,    1,  219,    2, 0x0a /* Public */,
      35,    0,  222,    2, 0x0a /* Public */,
      36,    1,  223,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::QString,    2,    2,
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
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   30,   31,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   34,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Par_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Par_Dialog *_t = static_cast<Par_Dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->table_change((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->par_cancel(); break;
        case 3: _t->show_detail((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->show_detail((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: _t->switch_to_operator((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->switch_to_debug((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->switch_to_develop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->getcellselect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->getuserDataChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->Key_Return(); break;
        case 11: _t->Key_O(); break;
        case 12: _t->Key_M(); break;
        case 13: _t->Key_D(); break;
        case 14: _t->Key_P(); break;
        case 15: _t->Key_F(); break;
        case 16: _t->Key_I(); break;
        case 17: _t->Key_H(); break;
        case 18: _t->Key_R(); break;
        case 19: _t->Key_V(); break;
        case 20: _t->Key_T(); break;
        case 21: _t->Key_Q(); break;
        case 22: _t->Key_S(); break;
        case 23: _t->getFindselect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: _t->ParaFind(); break;
        case 25: _t->slot_MenuGoto((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 26: _t->slot_TblGoto(); break;
        case 27: _t->tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Par_Dialog::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_Dialog::send_msg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Par_Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Par_Dialog.data,
      qt_meta_data_Par_Dialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Par_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Par_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Par_Dialog.stringdata0))
        return static_cast<void*>(const_cast< Par_Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Par_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void Par_Dialog::send_msg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CEditdlg_t {
    QByteArrayData data[8];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CEditdlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CEditdlg_t qt_meta_stringdata_CEditdlg = {
    {
QT_MOC_LITERAL(0, 0, 8), // "CEditdlg"
QT_MOC_LITERAL(1, 9, 11), // "setUserData"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "par_sure"
QT_MOC_LITERAL(4, 31, 10), // "par_cancel"
QT_MOC_LITERAL(5, 42, 8), // "par_copy"
QT_MOC_LITERAL(6, 51, 9), // "par_paste"
QT_MOC_LITERAL(7, 61, 11) // "par_getData"

    },
    "CEditdlg\0setUserData\0\0par_sure\0"
    "par_cancel\0par_copy\0par_paste\0par_getData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CEditdlg[] = {

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
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   49,    2, 0x0a /* Public */,
       4,    0,   50,    2, 0x0a /* Public */,
       5,    0,   51,    2, 0x0a /* Public */,
       6,    0,   52,    2, 0x0a /* Public */,
       7,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CEditdlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CEditdlg *_t = static_cast<CEditdlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setUserData((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->par_sure(); break;
        case 2: _t->par_cancel(); break;
        case 3: _t->par_copy(); break;
        case 4: _t->par_paste(); break;
        case 5: _t->par_getData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CEditdlg::*_t)(double , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CEditdlg::setUserData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CEditdlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CEditdlg.data,
      qt_meta_data_CEditdlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CEditdlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CEditdlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CEditdlg.stringdata0))
        return static_cast<void*>(const_cast< CEditdlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CEditdlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void CEditdlg::setUserData(double _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Par_txtEdit_t {
    QByteArrayData data[4];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Par_txtEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Par_txtEdit_t qt_meta_stringdata_Par_txtEdit = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Par_txtEdit"
QT_MOC_LITERAL(1, 12, 11), // "line_change"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3) // "str"

    },
    "Par_txtEdit\0line_change\0\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Par_txtEdit[] = {

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
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void Par_txtEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Par_txtEdit *_t = static_cast<Par_txtEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->line_change((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Par_txtEdit::staticMetaObject = {
    { &QDoubleSpinBox::staticMetaObject, qt_meta_stringdata_Par_txtEdit.data,
      qt_meta_data_Par_txtEdit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Par_txtEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Par_txtEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Par_txtEdit.stringdata0))
        return static_cast<void*>(const_cast< Par_txtEdit*>(this));
    return QDoubleSpinBox::qt_metacast(_clname);
}

int Par_txtEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDoubleSpinBox::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_Par_LineEdit_t {
    QByteArrayData data[9];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Par_LineEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Par_LineEdit_t qt_meta_stringdata_Par_LineEdit = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Par_LineEdit"
QT_MOC_LITERAL(1, 13, 11), // "valuechange"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "row"
QT_MOC_LITERAL(4, 30, 6), // "column"
QT_MOC_LITERAL(5, 37, 4), // "type"
QT_MOC_LITERAL(6, 42, 11), // "valueactive"
QT_MOC_LITERAL(7, 54, 11), // "line_change"
QT_MOC_LITERAL(8, 66, 12) // "line_change1"

    },
    "Par_LineEdit\0valuechange\0\0row\0column\0"
    "type\0valueactive\0line_change\0line_change1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Par_LineEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       6,    2,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   46,    2, 0x0a /* Public */,
       8,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void Par_LineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Par_LineEdit *_t = static_cast<Par_LineEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valuechange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->valueactive((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->line_change(); break;
        case 3: _t->line_change1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Par_LineEdit::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_LineEdit::valuechange)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Par_LineEdit::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_LineEdit::valueactive)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Par_LineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_Par_LineEdit.data,
      qt_meta_data_Par_LineEdit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Par_LineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Par_LineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Par_LineEdit.stringdata0))
        return static_cast<void*>(const_cast< Par_LineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int Par_LineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
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
void Par_LineEdit::valuechange(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Par_LineEdit::valueactive(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_Par_SpinBox_t {
    QByteArrayData data[8];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Par_SpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Par_SpinBox_t qt_meta_stringdata_Par_SpinBox = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Par_SpinBox"
QT_MOC_LITERAL(1, 12, 11), // "valuechange"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "row"
QT_MOC_LITERAL(4, 29, 6), // "column"
QT_MOC_LITERAL(5, 36, 4), // "type"
QT_MOC_LITERAL(6, 41, 11), // "valueactive"
QT_MOC_LITERAL(7, 53, 11) // "line_change"

    },
    "Par_SpinBox\0valuechange\0\0row\0column\0"
    "type\0valueactive\0line_change"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Par_SpinBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,
       6,    2,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Par_SpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Par_SpinBox *_t = static_cast<Par_SpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valuechange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->valueactive((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->line_change(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Par_SpinBox::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_SpinBox::valuechange)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Par_SpinBox::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_SpinBox::valueactive)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Par_SpinBox::staticMetaObject = {
    { &QSpinBox::staticMetaObject, qt_meta_stringdata_Par_SpinBox.data,
      qt_meta_data_Par_SpinBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Par_SpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Par_SpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Par_SpinBox.stringdata0))
        return static_cast<void*>(const_cast< Par_SpinBox*>(this));
    return QSpinBox::qt_metacast(_clname);
}

int Par_SpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSpinBox::qt_metacall(_c, _id, _a);
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
void Par_SpinBox::valuechange(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Par_SpinBox::valueactive(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_Par_DoubleSpinBox_t {
    QByteArrayData data[8];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Par_DoubleSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Par_DoubleSpinBox_t qt_meta_stringdata_Par_DoubleSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Par_DoubleSpinBox"
QT_MOC_LITERAL(1, 18, 11), // "valuechange"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "row"
QT_MOC_LITERAL(4, 35, 6), // "column"
QT_MOC_LITERAL(5, 42, 4), // "type"
QT_MOC_LITERAL(6, 47, 11), // "valueactive"
QT_MOC_LITERAL(7, 59, 11) // "line_change"

    },
    "Par_DoubleSpinBox\0valuechange\0\0row\0"
    "column\0type\0valueactive\0line_change"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Par_DoubleSpinBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,
       6,    2,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Par_DoubleSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Par_DoubleSpinBox *_t = static_cast<Par_DoubleSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valuechange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->valueactive((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->line_change(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Par_DoubleSpinBox::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_DoubleSpinBox::valuechange)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Par_DoubleSpinBox::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Par_DoubleSpinBox::valueactive)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Par_DoubleSpinBox::staticMetaObject = {
    { &QDoubleSpinBox::staticMetaObject, qt_meta_stringdata_Par_DoubleSpinBox.data,
      qt_meta_data_Par_DoubleSpinBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Par_DoubleSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Par_DoubleSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Par_DoubleSpinBox.stringdata0))
        return static_cast<void*>(const_cast< Par_DoubleSpinBox*>(this));
    return QDoubleSpinBox::qt_metacast(_clname);
}

int Par_DoubleSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDoubleSpinBox::qt_metacall(_c, _id, _a);
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
void Par_DoubleSpinBox::valuechange(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Par_DoubleSpinBox::valueactive(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
