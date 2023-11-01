/****************************************************************************
** Meta object code from reading C++ file 'allchannel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "newui/allchannel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'allchannel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_buildInstaller_t {
    QByteArrayData data[8];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_buildInstaller_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_buildInstaller_t qt_meta_stringdata_buildInstaller = {
    {
QT_MOC_LITERAL(0, 0, 14), // "buildInstaller"
QT_MOC_LITERAL(1, 15, 8), // "finished"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "buildinstall"
QT_MOC_LITERAL(4, 38, 10), // "get_system"
QT_MOC_LITERAL(5, 49, 3), // "cmd"
QT_MOC_LITERAL(6, 53, 8), // "QString*"
QT_MOC_LITERAL(7, 62, 3) // "ret"

    },
    "buildInstaller\0finished\0\0buildinstall\0"
    "get_system\0cmd\0QString*\0ret"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_buildInstaller[] = {

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
       3,    0,   37,    2, 0x0a /* Public */,
       4,    2,   38,    2, 0x0a /* Public */,
       4,    1,   43,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString, 0x80000000 | 6,    5,    7,
    QMetaType::Int, QMetaType::QString,    5,

       0        // eod
};

void buildInstaller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        buildInstaller *_t = static_cast<buildInstaller *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->buildinstall(); break;
        case 2: { int _r = _t->get_system((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->get_system((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (buildInstaller::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&buildInstaller::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject buildInstaller::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_buildInstaller.data,
      qt_meta_data_buildInstaller,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *buildInstaller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *buildInstaller::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_buildInstaller.stringdata0))
        return static_cast<void*>(const_cast< buildInstaller*>(this));
    return QObject::qt_metacast(_clname);
}

int buildInstaller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void buildInstaller::finished(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_BackupRestore_t {
    QByteArrayData data[4];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BackupRestore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BackupRestore_t qt_meta_stringdata_BackupRestore = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BackupRestore"
QT_MOC_LITERAL(1, 14, 8), // "finished"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16) // "BackupandRestore"

    },
    "BackupRestore\0finished\0\0BackupandRestore"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BackupRestore[] = {

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
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void BackupRestore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BackupRestore *_t = static_cast<BackupRestore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->BackupandRestore(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BackupRestore::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BackupRestore::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BackupRestore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BackupRestore.data,
      qt_meta_data_BackupRestore,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BackupRestore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BackupRestore::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BackupRestore.stringdata0))
        return static_cast<void*>(const_cast< BackupRestore*>(this));
    return QObject::qt_metacast(_clname);
}

int BackupRestore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void BackupRestore::finished(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CAMDIALOG_t {
    QByteArrayData data[5];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CAMDIALOG_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CAMDIALOG_t qt_meta_stringdata_CAMDIALOG = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CAMDIALOG"
QT_MOC_LITERAL(1, 10, 12), // "sendMsgToCam"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "closeEvent"
QT_MOC_LITERAL(4, 35, 12) // "QCloseEvent*"

    },
    "CAMDIALOG\0sendMsgToCam\0\0closeEvent\0"
    "QCloseEvent*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CAMDIALOG[] = {

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
       3,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 4,    2,

       0        // eod
};

void CAMDIALOG::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAMDIALOG *_t = static_cast<CAMDIALOG *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsgToCam((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CAMDIALOG::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAMDIALOG::sendMsgToCam)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CAMDIALOG::*_t)(QCloseEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAMDIALOG::closeEvent)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CAMDIALOG::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CAMDIALOG.data,
      qt_meta_data_CAMDIALOG,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CAMDIALOG::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CAMDIALOG::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CAMDIALOG.stringdata0))
        return static_cast<void*>(const_cast< CAMDIALOG*>(this));
    return QDialog::qt_metacast(_clname);
}

int CAMDIALOG::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void CAMDIALOG::sendMsgToCam(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CAMDIALOG::closeEvent(QCloseEvent * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_allchannel_t {
    QByteArrayData data[79];
    char stringdata0[1515];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_allchannel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_allchannel_t qt_meta_stringdata_allchannel = {
    {
QT_MOC_LITERAL(0, 0, 10), // "allchannel"
QT_MOC_LITERAL(1, 11, 15), // "hideAllCadTools"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "showAllCadTools"
QT_MOC_LITERAL(4, 44, 7), // "sendMsg"
QT_MOC_LITERAL(5, 52, 12), // "sendMsgToCam"
QT_MOC_LITERAL(6, 65, 9), // "updateCNC"
QT_MOC_LITERAL(7, 75, 6), // "InitUI"
QT_MOC_LITERAL(8, 82, 9), // "connectUI"
QT_MOC_LITERAL(9, 92, 10), // "runPressed"
QT_MOC_LITERAL(10, 103, 19), // "on_btn_stop_clicked"
QT_MOC_LITERAL(11, 123, 20), // "on_btn_reset_clicked"
QT_MOC_LITERAL(12, 144, 17), // "btn_auto_released"
QT_MOC_LITERAL(13, 162, 16), // "btn_cam_released"
QT_MOC_LITERAL(14, 179, 15), // "btn_xx_released"
QT_MOC_LITERAL(15, 195, 17), // "btn_para_released"
QT_MOC_LITERAL(16, 213, 17), // "btn_info_released"
QT_MOC_LITERAL(17, 231, 18), // "btn_other_released"
QT_MOC_LITERAL(18, 250, 22), // "on_btn_enlarge_clicked"
QT_MOC_LITERAL(19, 273, 26), // "on_btn_historyinfo_clicked"
QT_MOC_LITERAL(20, 300, 21), // "on_btn_update_clicked"
QT_MOC_LITERAL(21, 322, 23), // "on_btn_register_clicked"
QT_MOC_LITERAL(22, 346, 21), // "on_btn_backup_clicked"
QT_MOC_LITERAL(23, 368, 23), // "on_btn_backup_2_clicked"
QT_MOC_LITERAL(24, 392, 20), // "on_btn_motor_clicked"
QT_MOC_LITERAL(25, 413, 17), // "on_btn_io_clicked"
QT_MOC_LITERAL(26, 431, 23), // "on_btn_otherset_clicked"
QT_MOC_LITERAL(27, 455, 23), // "on_btn_halmeter_clicked"
QT_MOC_LITERAL(28, 479, 26), // "loadPositionManagerFromCam"
QT_MOC_LITERAL(29, 506, 19), // "set_board_thickness"
QT_MOC_LITERAL(30, 526, 16), // "set_board_height"
QT_MOC_LITERAL(31, 543, 13), // "set_frame_pos"
QT_MOC_LITERAL(32, 557, 16), // "set_side_2_width"
QT_MOC_LITERAL(33, 574, 16), // "set_side_3_width"
QT_MOC_LITERAL(34, 591, 16), // "set_cutextension"
QT_MOC_LITERAL(35, 608, 21), // "set_back_cutextension"
QT_MOC_LITERAL(36, 630, 18), // "set_back_off_space"
QT_MOC_LITERAL(37, 649, 18), // "set_down_off_space"
QT_MOC_LITERAL(38, 668, 18), // "set_backhole_board"
QT_MOC_LITERAL(39, 687, 13), // "set_use_belt2"
QT_MOC_LITERAL(40, 701, 16), // "set_side_1_width"
QT_MOC_LITERAL(41, 718, 15), // "set_Right_press"
QT_MOC_LITERAL(42, 734, 17), // "set_inhaler_space"
QT_MOC_LITERAL(43, 752, 19), // "set_inhaler_2_space"
QT_MOC_LITERAL(44, 772, 9), // "set_count"
QT_MOC_LITERAL(45, 782, 10), // "set_count2"
QT_MOC_LITERAL(46, 793, 20), // "on_btn_debug_clicked"
QT_MOC_LITERAL(47, 814, 22), // "on_btn_resetio_clicked"
QT_MOC_LITERAL(48, 837, 23), // "on_btn_backward_clicked"
QT_MOC_LITERAL(49, 861, 7), // "checked"
QT_MOC_LITERAL(50, 869, 23), // "on_chk_openBlow_clicked"
QT_MOC_LITERAL(51, 893, 26), // "on_chk_ignoreBoard_clicked"
QT_MOC_LITERAL(52, 920, 27), // "on_chk_useCollision_clicked"
QT_MOC_LITERAL(53, 948, 21), // "on_btn_repeat_clicked"
QT_MOC_LITERAL(54, 970, 14), // "messageFromCam"
QT_MOC_LITERAL(55, 985, 3), // "msg"
QT_MOC_LITERAL(56, 989, 23), // "on_btn_autoZoom_clicked"
QT_MOC_LITERAL(57, 1013, 18), // "displaySolutionsUI"
QT_MOC_LITERAL(58, 1032, 23), // "on_btn_showTool_clicked"
QT_MOC_LITERAL(59, 1056, 10), // "setblowout"
QT_MOC_LITERAL(60, 1067, 23), // "on_chk_toolback_clicked"
QT_MOC_LITERAL(61, 1091, 12), // "runDrillLeft"
QT_MOC_LITERAL(62, 1104, 13), // "runDrillRight"
QT_MOC_LITERAL(63, 1118, 24), // "on_btn_piecewise_clicked"
QT_MOC_LITERAL(64, 1143, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(65, 1165, 15), // "read_product_id"
QT_MOC_LITERAL(66, 1181, 26), // "on_chk_useBigPress_clicked"
QT_MOC_LITERAL(67, 1208, 33), // "on_chk_useBigPressForBack_cli..."
QT_MOC_LITERAL(68, 1242, 22), // "set_bigpress_leftLimit"
QT_MOC_LITERAL(69, 1265, 23), // "set_bigpress_rightLimit"
QT_MOC_LITERAL(70, 1289, 19), // "on_ModifyID_clicked"
QT_MOC_LITERAL(71, 1309, 17), // "on_SaveID_clicked"
QT_MOC_LITERAL(72, 1327, 30), // "on_chk_useLightCurtain_clicked"
QT_MOC_LITERAL(73, 1358, 26), // "on_chk_useWaitBack_clicked"
QT_MOC_LITERAL(74, 1385, 26), // "on_chk_changeBlock_clicked"
QT_MOC_LITERAL(75, 1412, 20), // "on_btn_print_clicked"
QT_MOC_LITERAL(76, 1433, 19), // "on_btn_warm_clicked"
QT_MOC_LITERAL(77, 1453, 35), // "on_chk_useLightCurtain_back_c..."
QT_MOC_LITERAL(78, 1489, 25) // "on_chk_useInhaler_clicked"

    },
    "allchannel\0hideAllCadTools\0\0showAllCadTools\0"
    "sendMsg\0sendMsgToCam\0updateCNC\0InitUI\0"
    "connectUI\0runPressed\0on_btn_stop_clicked\0"
    "on_btn_reset_clicked\0btn_auto_released\0"
    "btn_cam_released\0btn_xx_released\0"
    "btn_para_released\0btn_info_released\0"
    "btn_other_released\0on_btn_enlarge_clicked\0"
    "on_btn_historyinfo_clicked\0"
    "on_btn_update_clicked\0on_btn_register_clicked\0"
    "on_btn_backup_clicked\0on_btn_backup_2_clicked\0"
    "on_btn_motor_clicked\0on_btn_io_clicked\0"
    "on_btn_otherset_clicked\0on_btn_halmeter_clicked\0"
    "loadPositionManagerFromCam\0"
    "set_board_thickness\0set_board_height\0"
    "set_frame_pos\0set_side_2_width\0"
    "set_side_3_width\0set_cutextension\0"
    "set_back_cutextension\0set_back_off_space\0"
    "set_down_off_space\0set_backhole_board\0"
    "set_use_belt2\0set_side_1_width\0"
    "set_Right_press\0set_inhaler_space\0"
    "set_inhaler_2_space\0set_count\0set_count2\0"
    "on_btn_debug_clicked\0on_btn_resetio_clicked\0"
    "on_btn_backward_clicked\0checked\0"
    "on_chk_openBlow_clicked\0"
    "on_chk_ignoreBoard_clicked\0"
    "on_chk_useCollision_clicked\0"
    "on_btn_repeat_clicked\0messageFromCam\0"
    "msg\0on_btn_autoZoom_clicked\0"
    "displaySolutionsUI\0on_btn_showTool_clicked\0"
    "setblowout\0on_chk_toolback_clicked\0"
    "runDrillLeft\0runDrillRight\0"
    "on_btn_piecewise_clicked\0on_pushButton_clicked\0"
    "read_product_id\0on_chk_useBigPress_clicked\0"
    "on_chk_useBigPressForBack_clicked\0"
    "set_bigpress_leftLimit\0set_bigpress_rightLimit\0"
    "on_ModifyID_clicked\0on_SaveID_clicked\0"
    "on_chk_useLightCurtain_clicked\0"
    "on_chk_useWaitBack_clicked\0"
    "on_chk_changeBlock_clicked\0"
    "on_btn_print_clicked\0on_btn_warm_clicked\0"
    "on_chk_useLightCurtain_back_clicked\0"
    "on_chk_useInhaler_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allchannel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      75,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  389,    2, 0x06 /* Public */,
       3,    0,  390,    2, 0x06 /* Public */,
       4,    2,  391,    2, 0x06 /* Public */,
       5,    1,  396,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,  399,    2, 0x08 /* Private */,
       7,    0,  400,    2, 0x08 /* Private */,
       8,    0,  401,    2, 0x08 /* Private */,
       9,    0,  402,    2, 0x08 /* Private */,
      10,    0,  403,    2, 0x08 /* Private */,
      11,    0,  404,    2, 0x08 /* Private */,
      12,    0,  405,    2, 0x08 /* Private */,
      13,    0,  406,    2, 0x08 /* Private */,
      14,    0,  407,    2, 0x08 /* Private */,
      15,    0,  408,    2, 0x08 /* Private */,
      16,    0,  409,    2, 0x08 /* Private */,
      17,    0,  410,    2, 0x08 /* Private */,
      18,    0,  411,    2, 0x08 /* Private */,
      19,    0,  412,    2, 0x08 /* Private */,
      20,    0,  413,    2, 0x08 /* Private */,
      21,    0,  414,    2, 0x08 /* Private */,
      22,    0,  415,    2, 0x08 /* Private */,
      23,    0,  416,    2, 0x08 /* Private */,
      24,    0,  417,    2, 0x08 /* Private */,
      25,    0,  418,    2, 0x08 /* Private */,
      26,    0,  419,    2, 0x08 /* Private */,
      27,    0,  420,    2, 0x08 /* Private */,
      28,    0,  421,    2, 0x08 /* Private */,
      29,    0,  422,    2, 0x08 /* Private */,
      30,    0,  423,    2, 0x08 /* Private */,
      31,    0,  424,    2, 0x08 /* Private */,
      32,    0,  425,    2, 0x08 /* Private */,
      33,    0,  426,    2, 0x08 /* Private */,
      34,    0,  427,    2, 0x08 /* Private */,
      35,    0,  428,    2, 0x08 /* Private */,
      36,    0,  429,    2, 0x08 /* Private */,
      37,    0,  430,    2, 0x08 /* Private */,
      38,    0,  431,    2, 0x08 /* Private */,
      39,    0,  432,    2, 0x08 /* Private */,
      40,    0,  433,    2, 0x08 /* Private */,
      41,    0,  434,    2, 0x08 /* Private */,
      42,    0,  435,    2, 0x08 /* Private */,
      43,    0,  436,    2, 0x08 /* Private */,
      44,    0,  437,    2, 0x08 /* Private */,
      45,    0,  438,    2, 0x08 /* Private */,
      46,    0,  439,    2, 0x08 /* Private */,
      47,    0,  440,    2, 0x08 /* Private */,
      48,    1,  441,    2, 0x08 /* Private */,
      50,    0,  444,    2, 0x08 /* Private */,
      51,    0,  445,    2, 0x08 /* Private */,
      52,    0,  446,    2, 0x08 /* Private */,
      53,    1,  447,    2, 0x08 /* Private */,
      54,    1,  450,    2, 0x08 /* Private */,
      56,    0,  453,    2, 0x08 /* Private */,
      57,    0,  454,    2, 0x08 /* Private */,
      58,    0,  455,    2, 0x08 /* Private */,
      59,    0,  456,    2, 0x08 /* Private */,
      60,    0,  457,    2, 0x08 /* Private */,
      61,    0,  458,    2, 0x08 /* Private */,
      62,    0,  459,    2, 0x08 /* Private */,
      63,    0,  460,    2, 0x08 /* Private */,
      64,    0,  461,    2, 0x08 /* Private */,
      65,    0,  462,    2, 0x08 /* Private */,
      66,    0,  463,    2, 0x08 /* Private */,
      67,    0,  464,    2, 0x08 /* Private */,
      68,    0,  465,    2, 0x08 /* Private */,
      69,    0,  466,    2, 0x08 /* Private */,
      70,    1,  467,    2, 0x08 /* Private */,
      71,    0,  470,    2, 0x08 /* Private */,
      72,    0,  471,    2, 0x08 /* Private */,
      73,    0,  472,    2, 0x08 /* Private */,
      74,    0,  473,    2, 0x08 /* Private */,
      75,    0,  474,    2, 0x08 /* Private */,
      76,    0,  475,    2, 0x08 /* Private */,
      77,    0,  476,    2, 0x08 /* Private */,
      78,    0,  477,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

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
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::QString,   55,
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
    QMetaType::Void, QMetaType::Bool,   49,
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

void allchannel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allchannel *_t = static_cast<allchannel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hideAllCadTools(); break;
        case 1: _t->showAllCadTools(); break;
        case 2: _t->sendMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->sendMsgToCam((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->updateCNC(); break;
        case 5: _t->InitUI(); break;
        case 6: _t->connectUI(); break;
        case 7: _t->runPressed(); break;
        case 8: _t->on_btn_stop_clicked(); break;
        case 9: _t->on_btn_reset_clicked(); break;
        case 10: _t->btn_auto_released(); break;
        case 11: _t->btn_cam_released(); break;
        case 12: _t->btn_xx_released(); break;
        case 13: _t->btn_para_released(); break;
        case 14: _t->btn_info_released(); break;
        case 15: _t->btn_other_released(); break;
        case 16: _t->on_btn_enlarge_clicked(); break;
        case 17: _t->on_btn_historyinfo_clicked(); break;
        case 18: _t->on_btn_update_clicked(); break;
        case 19: _t->on_btn_register_clicked(); break;
        case 20: _t->on_btn_backup_clicked(); break;
        case 21: _t->on_btn_backup_2_clicked(); break;
        case 22: _t->on_btn_motor_clicked(); break;
        case 23: _t->on_btn_io_clicked(); break;
        case 24: _t->on_btn_otherset_clicked(); break;
        case 25: _t->on_btn_halmeter_clicked(); break;
        case 26: _t->loadPositionManagerFromCam(); break;
        case 27: _t->set_board_thickness(); break;
        case 28: _t->set_board_height(); break;
        case 29: _t->set_frame_pos(); break;
        case 30: _t->set_side_2_width(); break;
        case 31: _t->set_side_3_width(); break;
        case 32: _t->set_cutextension(); break;
        case 33: _t->set_back_cutextension(); break;
        case 34: _t->set_back_off_space(); break;
        case 35: _t->set_down_off_space(); break;
        case 36: _t->set_backhole_board(); break;
        case 37: _t->set_use_belt2(); break;
        case 38: _t->set_side_1_width(); break;
        case 39: _t->set_Right_press(); break;
        case 40: _t->set_inhaler_space(); break;
        case 41: _t->set_inhaler_2_space(); break;
        case 42: _t->set_count(); break;
        case 43: _t->set_count2(); break;
        case 44: _t->on_btn_debug_clicked(); break;
        case 45: _t->on_btn_resetio_clicked(); break;
        case 46: _t->on_btn_backward_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->on_chk_openBlow_clicked(); break;
        case 48: _t->on_chk_ignoreBoard_clicked(); break;
        case 49: _t->on_chk_useCollision_clicked(); break;
        case 50: _t->on_btn_repeat_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 51: _t->messageFromCam((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 52: _t->on_btn_autoZoom_clicked(); break;
        case 53: _t->displaySolutionsUI(); break;
        case 54: _t->on_btn_showTool_clicked(); break;
        case 55: _t->setblowout(); break;
        case 56: _t->on_chk_toolback_clicked(); break;
        case 57: _t->runDrillLeft(); break;
        case 58: _t->runDrillRight(); break;
        case 59: _t->on_btn_piecewise_clicked(); break;
        case 60: _t->on_pushButton_clicked(); break;
        case 61: _t->read_product_id(); break;
        case 62: _t->on_chk_useBigPress_clicked(); break;
        case 63: _t->on_chk_useBigPressForBack_clicked(); break;
        case 64: _t->set_bigpress_leftLimit(); break;
        case 65: _t->set_bigpress_rightLimit(); break;
        case 66: _t->on_ModifyID_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 67: _t->on_SaveID_clicked(); break;
        case 68: _t->on_chk_useLightCurtain_clicked(); break;
        case 69: _t->on_chk_useWaitBack_clicked(); break;
        case 70: _t->on_chk_changeBlock_clicked(); break;
        case 71: _t->on_btn_print_clicked(); break;
        case 72: _t->on_btn_warm_clicked(); break;
        case 73: _t->on_chk_useLightCurtain_back_clicked(); break;
        case 74: _t->on_chk_useInhaler_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (allchannel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allchannel::hideAllCadTools)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (allchannel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allchannel::showAllCadTools)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (allchannel::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allchannel::sendMsg)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (allchannel::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allchannel::sendMsgToCam)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject allchannel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_allchannel.data,
      qt_meta_data_allchannel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *allchannel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allchannel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_allchannel.stringdata0))
        return static_cast<void*>(const_cast< allchannel*>(this));
    return QWidget::qt_metacast(_clname);
}

int allchannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 75)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 75;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 75)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 75;
    }
    return _id;
}

// SIGNAL 0
void allchannel::hideAllCadTools()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void allchannel::showAllCadTools()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void allchannel::sendMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void allchannel::sendMsgToCam(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
