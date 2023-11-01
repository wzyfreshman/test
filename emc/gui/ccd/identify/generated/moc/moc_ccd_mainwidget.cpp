/****************************************************************************
** Meta object code from reading C++ file 'ccd_mainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/ccd_mainwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_mainwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCD_MainWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   15,   15,   15, 0x0a,
      65,   15,   15,   15, 0x0a,
      90,   15,   15,   15, 0x0a,
     115,   15,   15,   15, 0x0a,
     140,   15,   15,   15, 0x0a,
     159,   15,   15,   15, 0x0a,
     175,   15,   15,   15, 0x0a,
     191,   15,   15,   15, 0x0a,
     207,   15,   15,   15, 0x0a,
     223,   15,   15,   15, 0x0a,
     245,   15,   15,   15, 0x0a,
     267,   15,   15,   15, 0x0a,
     289,   15,   15,   15, 0x0a,
     311,   15,   15,   15, 0x0a,
     336,   15,   15,   15, 0x0a,
     362,   15,   15,   15, 0x0a,
     382,   15,   15,   15, 0x0a,
     408,  403,   15,   15, 0x0a,
     433,   15,   15,   15, 0x2a,
     451,  403,   15,   15, 0x0a,
     476,   15,   15,   15, 0x2a,
     494,   15,   15,   15, 0x0a,
     517,   15,   15,   15, 0x0a,
     536,   15,   15,   15, 0x08,
     565,   15,   15,   15, 0x08,
     594,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCD_MainWidget[] = {
    "CCD_MainWidget\0\0,\0show_msg(int,QString)\0"
    "slotChangeIdtifyIndex1()\0"
    "slotChangeIdtifyIndex2()\0"
    "slotChangeIdtifyIndex3()\0"
    "slotChangeIdtifyIndex4()\0slotIdentifyMark()\0"
    "slotEditMark1()\0slotEditMark2()\0"
    "slotEditMark3()\0slotEditMark4()\0"
    "slotGoMarkLocation1()\0slotGoMarkLocation2()\0"
    "slotGoMarkLocation3()\0slotGoMarkLocation4()\0"
    "actionEditSysParameter()\0"
    "actionEditSysParameter2()\0slotEditCommonPar()\0"
    "slotEditCommonPar2()\0file\0"
    "slotLoadSetting(QString)\0slotLoadSetting()\0"
    "slotSaveSetting(QString)\0slotSaveSetting()\0"
    "slotSimulateIdentify()\0slotUpdateTemDis()\0"
    "on_Btn_SavePattern_clicked()\0"
    "on_Btn_LoadPattern_clicked()\0"
    "on_Btn_SaveImg_clicked()\0"
};

void CCD_MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCD_MainWidget *_t = static_cast<CCD_MainWidget *>(_o);
        switch (_id) {
        case 0: _t->show_msg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->slotChangeIdtifyIndex1(); break;
        case 2: _t->slotChangeIdtifyIndex2(); break;
        case 3: _t->slotChangeIdtifyIndex3(); break;
        case 4: _t->slotChangeIdtifyIndex4(); break;
        case 5: _t->slotIdentifyMark(); break;
        case 6: _t->slotEditMark1(); break;
        case 7: _t->slotEditMark2(); break;
        case 8: _t->slotEditMark3(); break;
        case 9: _t->slotEditMark4(); break;
        case 10: _t->slotGoMarkLocation1(); break;
        case 11: _t->slotGoMarkLocation2(); break;
        case 12: _t->slotGoMarkLocation3(); break;
        case 13: _t->slotGoMarkLocation4(); break;
        case 14: _t->actionEditSysParameter(); break;
        case 15: _t->actionEditSysParameter2(); break;
        case 16: _t->slotEditCommonPar(); break;
        case 17: _t->slotEditCommonPar2(); break;
        case 18: _t->slotLoadSetting((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->slotLoadSetting(); break;
        case 20: _t->slotSaveSetting((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->slotSaveSetting(); break;
        case 22: _t->slotSimulateIdentify(); break;
        case 23: _t->slotUpdateTemDis(); break;
        case 24: _t->on_Btn_SavePattern_clicked(); break;
        case 25: _t->on_Btn_LoadPattern_clicked(); break;
        case 26: _t->on_Btn_SaveImg_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCD_MainWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCD_MainWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCD_MainWidget,
      qt_meta_data_CCD_MainWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCD_MainWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCD_MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCD_MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_MainWidget))
        return static_cast<void*>(const_cast< CCD_MainWidget*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< CCD_MainWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCD_MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void CCD_MainWidget::show_msg(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
