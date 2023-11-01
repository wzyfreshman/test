/****************************************************************************
** Meta object code from reading C++ file 'ccd_sysparcommoneditor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/ccd_sysparcommoneditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_sysparcommoneditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCD_SysParCommonEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x0a,
      35,   23,   23,   23, 0x0a,
      48,   23,   23,   23, 0x0a,
      73,   23,   23,   23, 0x0a,
     102,   23,   23,   23, 0x0a,
     126,   23,   23,   23, 0x0a,
     161,   23,   23,   23, 0x0a,
     194,   23,   23,   23, 0x0a,
     227,   23,   23,   23, 0x0a,
     263,   23,   23,   23, 0x0a,
     294,   23,   23,   23, 0x0a,
     338,  330,   23,   23, 0x08,
     364,  330,   23,   23, 0x08,
     392,  330,   23,   23, 0x08,
     419,  330,   23,   23, 0x08,
     447,  330,   23,   23, 0x08,
     480,  475,   23,   23, 0x08,
     518,  475,   23,   23, 0x08,
     556,  475,   23,   23, 0x08,
     596,  475,   23,   23, 0x08,
     637,   23,   23,   23, 0x08,
     668,  475,   23,   23, 0x08,
     710,  475,   23,   23, 0x08,
     742,  475,   23,   23, 0x08,
     776,  475,   23,   23, 0x08,
     819,  475,   23,   23, 0x08,
     862,  330,   23,   23, 0x08,
     892,  475,   23,   23, 0x08,
     926,  330,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCD_SysParCommonEditor[] = {
    "CCD_SysParCommonEditor\0\0slotSure()\0"
    "slotCancel()\0slotSetDelayTime(double)\0"
    "slotSetDistanceMerge(double)\0"
    "slotSetToolSize(double)\0"
    "slotSetCircleDistanceToler(double)\0"
    "slotSetCompensateSearchLen1(int)\0"
    "slotSetCompensateSearchLen2(int)\0"
    "slotSetCompensateSearchLen3(double)\0"
    "slotSetCompensateToler(double)\0"
    "slotSetCompensateAngleToler(double)\0"
    "checked\0on_Btn_TwoP_clicked(bool)\0"
    "on_Btn_ThreeP_clicked(bool)\0"
    "on_Btn_FourP_clicked(bool)\0"
    "on_Btn_LeftUp_clicked(bool)\0"
    "on_Btn_Center_clicked(bool)\0arg1\0"
    "on_SB_TwoPointsW_valueChanged(double)\0"
    "on_SB_TwoPointsH_valueChanged(double)\0"
    "on_LinE_ComputeLen_valueChanged(double)\0"
    "on_LinE_ComputeDIff_valueChanged(double)\0"
    "on_Btn_ComputeRotate_clicked()\0"
    "on_LinE_RotateOffset_valueChanged(double)\0"
    "on_Array_With_valueChanged(int)\0"
    "on_Array_Height_valueChanged(int)\0"
    "on_Array_Distance_Hor_valueChanged(double)\0"
    "on_Array_Distance_Ver_valueChanged(double)\0"
    "on_Array_Enable_clicked(bool)\0"
    "on_SB_SampleNum_valueChanged(int)\0"
    "on_Ckb_Offline_clicked(bool)\0"
};

void CCD_SysParCommonEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCD_SysParCommonEditor *_t = static_cast<CCD_SysParCommonEditor *>(_o);
        switch (_id) {
        case 0: _t->slotSure(); break;
        case 1: _t->slotCancel(); break;
        case 2: _t->slotSetDelayTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->slotSetDistanceMerge((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->slotSetToolSize((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->slotSetCircleDistanceToler((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->slotSetCompensateSearchLen1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slotSetCompensateSearchLen2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slotSetCompensateSearchLen3((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->slotSetCompensateToler((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->slotSetCompensateAngleToler((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->on_Btn_TwoP_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_Btn_ThreeP_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->on_Btn_FourP_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->on_Btn_LeftUp_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->on_Btn_Center_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->on_SB_TwoPointsW_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 17: _t->on_SB_TwoPointsH_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 18: _t->on_LinE_ComputeLen_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 19: _t->on_LinE_ComputeDIff_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 20: _t->on_Btn_ComputeRotate_clicked(); break;
        case 21: _t->on_LinE_RotateOffset_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 22: _t->on_Array_With_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->on_Array_Height_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->on_Array_Distance_Hor_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 25: _t->on_Array_Distance_Ver_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 26: _t->on_Array_Enable_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->on_SB_SampleNum_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->on_Ckb_Offline_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCD_SysParCommonEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCD_SysParCommonEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CCD_SysParCommonEditor,
      qt_meta_data_CCD_SysParCommonEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCD_SysParCommonEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCD_SysParCommonEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCD_SysParCommonEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_SysParCommonEditor))
        return static_cast<void*>(const_cast< CCD_SysParCommonEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int CCD_SysParCommonEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
