/****************************************************************************
** Meta object code from reading C++ file 'robot.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "robot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'robot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Robot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      20,    6,    6,    6, 0x08,
      36,    6,    6,    6, 0x08,
      47,    6,    6,    6, 0x08,
      65,    6,    6,    6, 0x08,
      81,    6,    6,    6, 0x08,
     108,  104,   97,    6, 0x08,
     126,  121,    6,    6, 0x08,
     141,    6,    6,    6, 0x08,
     166,    6,    6,    6, 0x08,
     191,    6,    6,    6, 0x08,
     218,    6,    6,    6, 0x08,
     244,    6,    6,    6, 0x08,
     275,    6,    6,    6, 0x08,
     308,    6,    6,    6, 0x08,
     337,    6,    6,    6, 0x08,
     368,    6,    6,    6, 0x08,
     395,    6,    6,    6, 0x08,
     424,    6,    6,    6, 0x08,
     455,    6,    6,    6, 0x08,
     486,    6,    6,    6, 0x08,
     519,    6,    6,    6, 0x08,
     552,    6,    6,    6, 0x08,
     576,    6,    6,    6, 0x08,
     602,    6,    6,    6, 0x08,
     628,    6,    6,    6, 0x08,
     656,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Robot[] = {
    "Robot\0\0widgetInit()\0parameterInit()\0"
    "updateUI()\0set_feed_rate_3()\0"
    "set_min_limit()\0set_max_limit()\0double\0"
    "var\0get_var(int)\0flag\0autoRobot(int)\0"
    "on_btn_rc_work_clicked()\0"
    "on_btn_rc_load_clicked()\0"
    "on_btn_rc_unload_clicked()\0"
    "on_btn_rc_clear_clicked()\0"
    "on_btn_loadCylinder_released()\0"
    "on_btn_unloadCylinder_released()\0"
    "on_btn_loadVacuum_released()\0"
    "on_btn_unloadVacuum_released()\0"
    "on_btn_loadBlow_released()\0"
    "on_btn_unloadBlow_released()\0"
    "on_btn_tool1_vacuum_released()\0"
    "on_btn_tool2_vacuum_released()\0"
    "on_btn_break_vacuum_1_released()\0"
    "on_btn_break_vacuum_2_released()\0"
    "on_btn_debug_released()\0"
    "on_btn_work_pos_clicked()\0"
    "on_btn_load_pos_clicked()\0"
    "on_btn_unload_pos_clicked()\0"
    "on_btn_clear_pos_clicked()\0"
};

void Robot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Robot *_t = static_cast<Robot *>(_o);
        switch (_id) {
        case 0: _t->widgetInit(); break;
        case 1: _t->parameterInit(); break;
        case 2: _t->updateUI(); break;
        case 3: _t->set_feed_rate_3(); break;
        case 4: _t->set_min_limit(); break;
        case 5: _t->set_max_limit(); break;
        case 6: { double _r = _t->get_var((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 7: _t->autoRobot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_btn_rc_work_clicked(); break;
        case 9: _t->on_btn_rc_load_clicked(); break;
        case 10: _t->on_btn_rc_unload_clicked(); break;
        case 11: _t->on_btn_rc_clear_clicked(); break;
        case 12: _t->on_btn_loadCylinder_released(); break;
        case 13: _t->on_btn_unloadCylinder_released(); break;
        case 14: _t->on_btn_loadVacuum_released(); break;
        case 15: _t->on_btn_unloadVacuum_released(); break;
        case 16: _t->on_btn_loadBlow_released(); break;
        case 17: _t->on_btn_unloadBlow_released(); break;
        case 18: _t->on_btn_tool1_vacuum_released(); break;
        case 19: _t->on_btn_tool2_vacuum_released(); break;
        case 20: _t->on_btn_break_vacuum_1_released(); break;
        case 21: _t->on_btn_break_vacuum_2_released(); break;
        case 22: _t->on_btn_debug_released(); break;
        case 23: _t->on_btn_work_pos_clicked(); break;
        case 24: _t->on_btn_load_pos_clicked(); break;
        case 25: _t->on_btn_unload_pos_clicked(); break;
        case 26: _t->on_btn_clear_pos_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Robot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Robot::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Robot,
      qt_meta_data_Robot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Robot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Robot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Robot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Robot))
        return static_cast<void*>(const_cast< Robot*>(this));
    return QDialog::qt_metacast(_clname);
}

int Robot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
