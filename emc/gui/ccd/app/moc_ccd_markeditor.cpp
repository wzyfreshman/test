/****************************************************************************
** Meta object code from reading C++ file 'ccd_markeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/ccd_markeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_markeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_MarkEditor_t {
    QByteArrayData data[80];
    char stringdata0[1828];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_MarkEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_MarkEditor_t qt_meta_stringdata_CCD_MarkEditor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CCD_MarkEditor"
QT_MOC_LITERAL(1, 15, 20), // "slotDoSelectTemplate"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 15), // "slotSetAnglePos"
QT_MOC_LITERAL(4, 53, 12), // "CD::AnglePos"
QT_MOC_LITERAL(5, 66, 15), // "slotTestFeature"
QT_MOC_LITERAL(6, 82, 15), // "slotTryIdentify"
QT_MOC_LITERAL(7, 98, 19), // "slotSetOperModeAuto"
QT_MOC_LITERAL(8, 118, 21), // "slotSetOperModeManual"
QT_MOC_LITERAL(9, 140, 19), // "slotSetFeatureAngle"
QT_MOC_LITERAL(10, 160, 20), // "slotSetFeatureCircle"
QT_MOC_LITERAL(11, 181, 20), // "slotSetFeatureRelate"
QT_MOC_LITERAL(12, 202, 19), // "slotSetFeatureCross"
QT_MOC_LITERAL(13, 222, 19), // "slotSetFeatureShape"
QT_MOC_LITERAL(14, 242, 18), // "slotTrigleAnglePos"
QT_MOC_LITERAL(15, 261, 18), // "slotUpdateFeatures"
QT_MOC_LITERAL(16, 280, 16), // "slotFeatureClick"
QT_MOC_LITERAL(17, 297, 21), // "slotDetermineTemplate"
QT_MOC_LITERAL(18, 319, 21), // "slotForceReturnSelect"
QT_MOC_LITERAL(19, 341, 20), // "slotDetermineFeature"
QT_MOC_LITERAL(20, 362, 16), // "contextMenuEvent"
QT_MOC_LITERAL(21, 379, 18), // "QContextMenuEvent*"
QT_MOC_LITERAL(22, 398, 15), // "releaseFeatures"
QT_MOC_LITERAL(23, 414, 14), // "displayFeature"
QT_MOC_LITERAL(24, 429, 19), // "QList<CD_Feature*>&"
QT_MOC_LITERAL(25, 449, 3), // "fts"
QT_MOC_LITERAL(26, 453, 21), // "releaseListWidgetData"
QT_MOC_LITERAL(27, 475, 26), // "on_CameraGain_valueChanged"
QT_MOC_LITERAL(28, 502, 5), // "value"
QT_MOC_LITERAL(29, 508, 32), // "on_CameraExposeTime_valueChanged"
QT_MOC_LITERAL(30, 541, 30), // "on_CameraContrast_valueChanged"
QT_MOC_LITERAL(31, 572, 27), // "on_CameraGamma_valueChanged"
QT_MOC_LITERAL(32, 600, 17), // "on_Light1_clicked"
QT_MOC_LITERAL(33, 618, 7), // "checked"
QT_MOC_LITERAL(34, 626, 17), // "on_Light2_clicked"
QT_MOC_LITERAL(35, 644, 17), // "on_Light3_clicked"
QT_MOC_LITERAL(36, 662, 17), // "on_Light1_toggled"
QT_MOC_LITERAL(37, 680, 17), // "on_Light2_toggled"
QT_MOC_LITERAL(38, 698, 17), // "on_Light3_toggled"
QT_MOC_LITERAL(39, 716, 24), // "on_LightPar_Save_clicked"
QT_MOC_LITERAL(40, 741, 28), // "on_SB_SearchLen_valueChanged"
QT_MOC_LITERAL(41, 770, 4), // "arg1"
QT_MOC_LITERAL(42, 775, 32), // "on_SB_Sim_Threshold_valueChanged"
QT_MOC_LITERAL(43, 808, 22), // "on_SB_Fat_valueChanged"
QT_MOC_LITERAL(44, 831, 24), // "on_CKB_Calcenter_clicked"
QT_MOC_LITERAL(45, 856, 31), // "on_SB_Wh_Threshold_valueChanged"
QT_MOC_LITERAL(46, 888, 23), // "on_Btn_GetShape_clicked"
QT_MOC_LITERAL(47, 912, 27), // "on_Btn_ReturnOriImg_clicked"
QT_MOC_LITERAL(48, 940, 34), // "on_DB_SimThreshold_SH_valueCh..."
QT_MOC_LITERAL(49, 975, 30), // "on_Canny_Threhold_valueChanged"
QT_MOC_LITERAL(50, 1006, 26), // "on_Btn_Mode_Change_clicked"
QT_MOC_LITERAL(51, 1033, 24), // "on_Type_Relate_2_toggled"
QT_MOC_LITERAL(52, 1058, 24), // "on_Type_Circle_2_toggled"
QT_MOC_LITERAL(53, 1083, 23), // "on_Type_Angle_2_toggled"
QT_MOC_LITERAL(54, 1107, 23), // "on_Type_Cross_2_toggled"
QT_MOC_LITERAL(55, 1131, 26), // "on_Btn_CalRefPoint_clicked"
QT_MOC_LITERAL(56, 1158, 27), // "on_Btn_CalRefPoint2_clicked"
QT_MOC_LITERAL(57, 1186, 27), // "on_Search_Step_valueChanged"
QT_MOC_LITERAL(58, 1214, 28), // "on_Search_PyNum_valueChanged"
QT_MOC_LITERAL(59, 1243, 31), // "on_Search_AngleNbh_valueChanged"
QT_MOC_LITERAL(60, 1275, 30), // "on_Search_RectNbh_valueChanged"
QT_MOC_LITERAL(61, 1306, 22), // "on_Btn_MorePar_clicked"
QT_MOC_LITERAL(62, 1329, 19), // "on_EditMore_clicked"
QT_MOC_LITERAL(63, 1349, 19), // "on_Img_Last_clicked"
QT_MOC_LITERAL(64, 1369, 19), // "on_Img_Next_clicked"
QT_MOC_LITERAL(65, 1389, 22), // "on_Btn_Enraser_clicked"
QT_MOC_LITERAL(66, 1412, 28), // "on_Enraser_Size_valueChanged"
QT_MOC_LITERAL(67, 1441, 31), // "on_SB_CameraContra_valueChanged"
QT_MOC_LITERAL(68, 1473, 29), // "on_SB_ExposeTime_valueChanged"
QT_MOC_LITERAL(69, 1503, 29), // "on_SB_CameraGain_valueChanged"
QT_MOC_LITERAL(70, 1533, 30), // "on_SB_CameraGamma_valueChanged"
QT_MOC_LITERAL(71, 1564, 31), // "on_Search_Enable_Filter_clicked"
QT_MOC_LITERAL(72, 1596, 32), // "on_Search_Blur_Size_valueChanged"
QT_MOC_LITERAL(73, 1629, 35), // "on_Search_Erosion_Size_valueC..."
QT_MOC_LITERAL(74, 1665, 29), // "on_Gray_Asisst_Enable_clicked"
QT_MOC_LITERAL(75, 1695, 25), // "on_Btn_EditBitImg_clicked"
QT_MOC_LITERAL(76, 1721, 26), // "on_AreaSelPar_Save_clicked"
QT_MOC_LITERAL(77, 1748, 27), // "on_exposeCount_valueChanged"
QT_MOC_LITERAL(78, 1776, 27), // "on_exposeValue_valueChanged"
QT_MOC_LITERAL(79, 1804, 23) // "on_MarkPosSaved_clicked"

    },
    "CCD_MarkEditor\0slotDoSelectTemplate\0"
    "\0slotSetAnglePos\0CD::AnglePos\0"
    "slotTestFeature\0slotTryIdentify\0"
    "slotSetOperModeAuto\0slotSetOperModeManual\0"
    "slotSetFeatureAngle\0slotSetFeatureCircle\0"
    "slotSetFeatureRelate\0slotSetFeatureCross\0"
    "slotSetFeatureShape\0slotTrigleAnglePos\0"
    "slotUpdateFeatures\0slotFeatureClick\0"
    "slotDetermineTemplate\0slotForceReturnSelect\0"
    "slotDetermineFeature\0contextMenuEvent\0"
    "QContextMenuEvent*\0releaseFeatures\0"
    "displayFeature\0QList<CD_Feature*>&\0"
    "fts\0releaseListWidgetData\0"
    "on_CameraGain_valueChanged\0value\0"
    "on_CameraExposeTime_valueChanged\0"
    "on_CameraContrast_valueChanged\0"
    "on_CameraGamma_valueChanged\0"
    "on_Light1_clicked\0checked\0on_Light2_clicked\0"
    "on_Light3_clicked\0on_Light1_toggled\0"
    "on_Light2_toggled\0on_Light3_toggled\0"
    "on_LightPar_Save_clicked\0"
    "on_SB_SearchLen_valueChanged\0arg1\0"
    "on_SB_Sim_Threshold_valueChanged\0"
    "on_SB_Fat_valueChanged\0on_CKB_Calcenter_clicked\0"
    "on_SB_Wh_Threshold_valueChanged\0"
    "on_Btn_GetShape_clicked\0"
    "on_Btn_ReturnOriImg_clicked\0"
    "on_DB_SimThreshold_SH_valueChanged\0"
    "on_Canny_Threhold_valueChanged\0"
    "on_Btn_Mode_Change_clicked\0"
    "on_Type_Relate_2_toggled\0"
    "on_Type_Circle_2_toggled\0"
    "on_Type_Angle_2_toggled\0on_Type_Cross_2_toggled\0"
    "on_Btn_CalRefPoint_clicked\0"
    "on_Btn_CalRefPoint2_clicked\0"
    "on_Search_Step_valueChanged\0"
    "on_Search_PyNum_valueChanged\0"
    "on_Search_AngleNbh_valueChanged\0"
    "on_Search_RectNbh_valueChanged\0"
    "on_Btn_MorePar_clicked\0on_EditMore_clicked\0"
    "on_Img_Last_clicked\0on_Img_Next_clicked\0"
    "on_Btn_Enraser_clicked\0"
    "on_Enraser_Size_valueChanged\0"
    "on_SB_CameraContra_valueChanged\0"
    "on_SB_ExposeTime_valueChanged\0"
    "on_SB_CameraGain_valueChanged\0"
    "on_SB_CameraGamma_valueChanged\0"
    "on_Search_Enable_Filter_clicked\0"
    "on_Search_Blur_Size_valueChanged\0"
    "on_Search_Erosion_Size_valueChanged\0"
    "on_Gray_Asisst_Enable_clicked\0"
    "on_Btn_EditBitImg_clicked\0"
    "on_AreaSelPar_Save_clicked\0"
    "on_exposeCount_valueChanged\0"
    "on_exposeValue_valueChanged\0"
    "on_MarkPosSaved_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_MarkEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      72,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  374,    2, 0x0a /* Public */,
       3,    1,  375,    2, 0x0a /* Public */,
       5,    0,  378,    2, 0x0a /* Public */,
       6,    0,  379,    2, 0x0a /* Public */,
       7,    0,  380,    2, 0x0a /* Public */,
       8,    0,  381,    2, 0x0a /* Public */,
       9,    0,  382,    2, 0x0a /* Public */,
      10,    0,  383,    2, 0x0a /* Public */,
      11,    0,  384,    2, 0x0a /* Public */,
      12,    0,  385,    2, 0x0a /* Public */,
      13,    0,  386,    2, 0x0a /* Public */,
      14,    0,  387,    2, 0x0a /* Public */,
      15,    0,  388,    2, 0x0a /* Public */,
      16,    0,  389,    2, 0x0a /* Public */,
      16,    1,  390,    2, 0x0a /* Public */,
      17,    0,  393,    2, 0x0a /* Public */,
      18,    0,  394,    2, 0x0a /* Public */,
      19,    0,  395,    2, 0x0a /* Public */,
      20,    1,  396,    2, 0x0a /* Public */,
      22,    0,  399,    2, 0x0a /* Public */,
      23,    1,  400,    2, 0x0a /* Public */,
      26,    0,  403,    2, 0x0a /* Public */,
      27,    1,  404,    2, 0x08 /* Private */,
      29,    1,  407,    2, 0x08 /* Private */,
      30,    1,  410,    2, 0x08 /* Private */,
      31,    1,  413,    2, 0x08 /* Private */,
      32,    1,  416,    2, 0x08 /* Private */,
      34,    1,  419,    2, 0x08 /* Private */,
      35,    1,  422,    2, 0x08 /* Private */,
      36,    1,  425,    2, 0x08 /* Private */,
      37,    1,  428,    2, 0x08 /* Private */,
      38,    1,  431,    2, 0x08 /* Private */,
      39,    0,  434,    2, 0x08 /* Private */,
      40,    1,  435,    2, 0x08 /* Private */,
      42,    1,  438,    2, 0x08 /* Private */,
      43,    1,  441,    2, 0x08 /* Private */,
      44,    1,  444,    2, 0x08 /* Private */,
      45,    1,  447,    2, 0x08 /* Private */,
      46,    0,  450,    2, 0x08 /* Private */,
      47,    0,  451,    2, 0x08 /* Private */,
      48,    1,  452,    2, 0x08 /* Private */,
      49,    1,  455,    2, 0x08 /* Private */,
      50,    0,  458,    2, 0x08 /* Private */,
      51,    1,  459,    2, 0x08 /* Private */,
      52,    1,  462,    2, 0x08 /* Private */,
      53,    1,  465,    2, 0x08 /* Private */,
      54,    1,  468,    2, 0x08 /* Private */,
      55,    0,  471,    2, 0x08 /* Private */,
      56,    0,  472,    2, 0x08 /* Private */,
      57,    1,  473,    2, 0x08 /* Private */,
      58,    1,  476,    2, 0x08 /* Private */,
      59,    1,  479,    2, 0x08 /* Private */,
      60,    1,  482,    2, 0x08 /* Private */,
      61,    0,  485,    2, 0x08 /* Private */,
      62,    1,  486,    2, 0x08 /* Private */,
      63,    0,  489,    2, 0x08 /* Private */,
      64,    0,  490,    2, 0x08 /* Private */,
      65,    1,  491,    2, 0x08 /* Private */,
      66,    1,  494,    2, 0x08 /* Private */,
      67,    1,  497,    2, 0x08 /* Private */,
      68,    1,  500,    2, 0x08 /* Private */,
      69,    1,  503,    2, 0x08 /* Private */,
      70,    1,  506,    2, 0x08 /* Private */,
      71,    1,  509,    2, 0x08 /* Private */,
      72,    1,  512,    2, 0x08 /* Private */,
      73,    1,  515,    2, 0x08 /* Private */,
      74,    1,  518,    2, 0x08 /* Private */,
      75,    0,  521,    2, 0x08 /* Private */,
      76,    0,  522,    2, 0x08 /* Private */,
      77,    1,  523,    2, 0x08 /* Private */,
      78,    1,  526,    2, 0x08 /* Private */,
      79,    0,  529,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
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
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Double,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Double,   41,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   41,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Double,   41,
    QMetaType::Void,

       0        // eod
};

void CCD_MarkEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_MarkEditor *_t = static_cast<CCD_MarkEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotDoSelectTemplate(); break;
        case 1: _t->slotSetAnglePos((*reinterpret_cast< CD::AnglePos(*)>(_a[1]))); break;
        case 2: _t->slotTestFeature(); break;
        case 3: _t->slotTryIdentify(); break;
        case 4: _t->slotSetOperModeAuto(); break;
        case 5: _t->slotSetOperModeManual(); break;
        case 6: _t->slotSetFeatureAngle(); break;
        case 7: _t->slotSetFeatureCircle(); break;
        case 8: _t->slotSetFeatureRelate(); break;
        case 9: _t->slotSetFeatureCross(); break;
        case 10: _t->slotSetFeatureShape(); break;
        case 11: _t->slotTrigleAnglePos(); break;
        case 12: _t->slotUpdateFeatures(); break;
        case 13: _t->slotFeatureClick(); break;
        case 14: _t->slotFeatureClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->slotDetermineTemplate(); break;
        case 16: _t->slotForceReturnSelect(); break;
        case 17: _t->slotDetermineFeature(); break;
        case 18: _t->contextMenuEvent((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 19: _t->releaseFeatures(); break;
        case 20: _t->displayFeature((*reinterpret_cast< QList<CD_Feature*>(*)>(_a[1]))); break;
        case 21: _t->releaseListWidgetData(); break;
        case 22: _t->on_CameraGain_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->on_CameraExposeTime_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->on_CameraContrast_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->on_CameraGamma_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->on_Light1_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->on_Light2_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->on_Light3_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->on_Light1_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->on_Light2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->on_Light3_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: _t->on_LightPar_Save_clicked(); break;
        case 33: _t->on_SB_SearchLen_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->on_SB_Sim_Threshold_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 35: _t->on_SB_Fat_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->on_CKB_Calcenter_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->on_SB_Wh_Threshold_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 38: _t->on_Btn_GetShape_clicked(); break;
        case 39: _t->on_Btn_ReturnOriImg_clicked(); break;
        case 40: _t->on_DB_SimThreshold_SH_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 41: _t->on_Canny_Threhold_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->on_Btn_Mode_Change_clicked(); break;
        case 43: _t->on_Type_Relate_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->on_Type_Circle_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 45: _t->on_Type_Angle_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 46: _t->on_Type_Cross_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->on_Btn_CalRefPoint_clicked(); break;
        case 48: _t->on_Btn_CalRefPoint2_clicked(); break;
        case 49: _t->on_Search_Step_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 50: _t->on_Search_PyNum_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 51: _t->on_Search_AngleNbh_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: _t->on_Search_RectNbh_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->on_Btn_MorePar_clicked(); break;
        case 54: _t->on_EditMore_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->on_Img_Last_clicked(); break;
        case 56: _t->on_Img_Next_clicked(); break;
        case 57: _t->on_Btn_Enraser_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 58: _t->on_Enraser_Size_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 59: _t->on_SB_CameraContra_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 60: _t->on_SB_ExposeTime_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: _t->on_SB_CameraGain_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 62: _t->on_SB_CameraGamma_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 63: _t->on_Search_Enable_Filter_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 64: _t->on_Search_Blur_Size_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 65: _t->on_Search_Erosion_Size_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 66: _t->on_Gray_Asisst_Enable_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 67: _t->on_Btn_EditBitImg_clicked(); break;
        case 68: _t->on_AreaSelPar_Save_clicked(); break;
        case 69: _t->on_exposeCount_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 70: _t->on_exposeValue_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 71: _t->on_MarkPosSaved_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CCD_MarkEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CCD_MarkEditor.data,
      qt_meta_data_CCD_MarkEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_MarkEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_MarkEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_MarkEditor.stringdata0))
        return static_cast<void*>(const_cast< CCD_MarkEditor*>(this));
    if (!strcmp(_clname, "CCD_MarksFace"))
        return static_cast< CCD_MarksFace*>(const_cast< CCD_MarkEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int CCD_MarkEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 72)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 72;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 72)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 72;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
