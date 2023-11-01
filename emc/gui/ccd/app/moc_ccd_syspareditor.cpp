/****************************************************************************
** Meta object code from reading C++ file 'ccd_syspareditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/ccd_syspareditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccd_syspareditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCD_SysParEditor_t {
    QByteArrayData data[89];
    char stringdata0[1918];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCD_SysParEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCD_SysParEditor_t qt_meta_stringdata_CCD_SysParEditor = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CCD_SysParEditor"
QT_MOC_LITERAL(1, 17, 8), // "slotSure"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "slotCancel"
QT_MOC_LITERAL(4, 38, 16), // "slotTurnToCamera"
QT_MOC_LITERAL(5, 55, 19), // "slotTurnToCCDHeight"
QT_MOC_LITERAL(6, 75, 16), // "slotTurnToCCDMag"
QT_MOC_LITERAL(7, 92, 19), // "slotTurnToCCDOffset"
QT_MOC_LITERAL(8, 112, 14), // "slotTurnToAuto"
QT_MOC_LITERAL(9, 127, 11), // "slotGetMark"
QT_MOC_LITERAL(10, 139, 5), // "index"
QT_MOC_LITERAL(11, 145, 21), // "slotAutoLearnIdentify"
QT_MOC_LITERAL(12, 167, 16), // "activeMainScreen"
QT_MOC_LITERAL(13, 184, 14), // "slotCameraGain"
QT_MOC_LITERAL(14, 199, 17), // "slotCameraShutter"
QT_MOC_LITERAL(15, 217, 18), // "slotCameraContrast"
QT_MOC_LITERAL(16, 236, 20), // "slotSetCameraMirrorV"
QT_MOC_LITERAL(17, 257, 20), // "slotSetCameraMirrorH"
QT_MOC_LITERAL(18, 278, 19), // "slotSetCameraHeight"
QT_MOC_LITERAL(19, 298, 15), // "slotReadCameraH"
QT_MOC_LITERAL(20, 314, 14), // "slotFetchImage"
QT_MOC_LITERAL(21, 329, 11), // "slotShowMag"
QT_MOC_LITERAL(22, 341, 12), // "slotCheckAll"
QT_MOC_LITERAL(23, 354, 11), // "slotSetMagX"
QT_MOC_LITERAL(24, 366, 11), // "slotSetMagY"
QT_MOC_LITERAL(25, 378, 10), // "slotIdPos1"
QT_MOC_LITERAL(26, 389, 10), // "slotIdPos2"
QT_MOC_LITERAL(27, 400, 10), // "slotIdPos3"
QT_MOC_LITERAL(28, 411, 10), // "slotIdPos4"
QT_MOC_LITERAL(29, 422, 14), // "slotComputeMag"
QT_MOC_LITERAL(30, 437, 14), // "slotSetOffsetX"
QT_MOC_LITERAL(31, 452, 14), // "slotSetOffsetY"
QT_MOC_LITERAL(32, 467, 18), // "slotSetOffsetAngle"
QT_MOC_LITERAL(33, 486, 19), // "slotIdMarkForOffset"
QT_MOC_LITERAL(34, 506, 20), // "slotGetMarkForOffset"
QT_MOC_LITERAL(35, 527, 20), // "slotIdMarkForOffset1"
QT_MOC_LITERAL(36, 548, 20), // "slotIdMarkForOffset2"
QT_MOC_LITERAL(37, 569, 20), // "slotIdMarkForOffset3"
QT_MOC_LITERAL(38, 590, 20), // "slotIdMarkForOffset4"
QT_MOC_LITERAL(39, 611, 20), // "slotIdMarkForOffset5"
QT_MOC_LITERAL(40, 632, 20), // "slotIdMarkForOffset6"
QT_MOC_LITERAL(41, 653, 27), // "slotCalculateCenterPointAvg"
QT_MOC_LITERAL(42, 681, 31), // "slotCalculateCenterPointAvgAuto"
QT_MOC_LITERAL(43, 713, 25), // "slotCalculateCenterPoints"
QT_MOC_LITERAL(44, 739, 12), // "slotCncAbort"
QT_MOC_LITERAL(45, 752, 12), // "slotSaveMark"
QT_MOC_LITERAL(46, 765, 3), // "log"
QT_MOC_LITERAL(47, 769, 3), // "str"
QT_MOC_LITERAL(48, 773, 21), // "on_Btn_GetPos_clicked"
QT_MOC_LITERAL(49, 795, 29), // "on_Auto_PreMoveY_valueChanged"
QT_MOC_LITERAL(50, 825, 4), // "arg1"
QT_MOC_LITERAL(51, 830, 29), // "on_Auto_PreMoveA_valueChanged"
QT_MOC_LITERAL(52, 860, 25), // "on_Auto_MinA_valueChanged"
QT_MOC_LITERAL(53, 886, 25), // "on_Auto_MaxA_valueChanged"
QT_MOC_LITERAL(54, 912, 24), // "on_Btn_AutoStart_clicked"
QT_MOC_LITERAL(55, 937, 24), // "on_Btn_AutoReset_clicked"
QT_MOC_LITERAL(56, 962, 26), // "on_Btn_GetAutoMark_clicked"
QT_MOC_LITERAL(57, 989, 34), // "on_Auto_IdentifyDelay_valueCh..."
QT_MOC_LITERAL(58, 1024, 40), // "on_Auto_PlatformType_currentI..."
QT_MOC_LITERAL(59, 1065, 27), // "on_Btn_AutoIdentify_clicked"
QT_MOC_LITERAL(60, 1093, 29), // "on_Auto_PreMoveX_valueChanged"
QT_MOC_LITERAL(61, 1123, 25), // "on_Test_Num1_valueChanged"
QT_MOC_LITERAL(62, 1149, 25), // "on_Test_Num2_valueChanged"
QT_MOC_LITERAL(63, 1175, 25), // "on_Test_Num3_valueChanged"
QT_MOC_LITERAL(64, 1201, 30), // "on_Btn_EnableDirectPos_clicked"
QT_MOC_LITERAL(65, 1232, 29), // "on_Btn_EnableSmartPos_clicked"
QT_MOC_LITERAL(66, 1262, 25), // "on_Dir_Max_A_valueChanged"
QT_MOC_LITERAL(67, 1288, 25), // "on_Dir_Min_A_valueChanged"
QT_MOC_LITERAL(68, 1314, 26), // "on_Dir_Mag_X1_valueChanged"
QT_MOC_LITERAL(69, 1341, 26), // "on_Dir_Mag_X2_valueChanged"
QT_MOC_LITERAL(70, 1368, 26), // "on_Dir_Mag_Y2_valueChanged"
QT_MOC_LITERAL(71, 1395, 26), // "on_Dir_Mag_X4_valueChanged"
QT_MOC_LITERAL(72, 1422, 26), // "on_Dir_Mag_X3_valueChanged"
QT_MOC_LITERAL(73, 1449, 26), // "on_Dir_Mag_Y4_valueChanged"
QT_MOC_LITERAL(74, 1476, 26), // "on_Dir_Mag_Y3_valueChanged"
QT_MOC_LITERAL(75, 1503, 26), // "on_Dir_Mag_Y1_valueChanged"
QT_MOC_LITERAL(76, 1530, 29), // "on_Dir_Offset_X1_valueChanged"
QT_MOC_LITERAL(77, 1560, 29), // "on_Dir_Offset_X2_valueChanged"
QT_MOC_LITERAL(78, 1590, 29), // "on_Dir_Offset_X3_valueChanged"
QT_MOC_LITERAL(79, 1620, 29), // "on_Dir_Offset_X4_valueChanged"
QT_MOC_LITERAL(80, 1650, 29), // "on_Dir_Offset_X5_valueChanged"
QT_MOC_LITERAL(81, 1680, 29), // "on_Dir_Offset_X6_valueChanged"
QT_MOC_LITERAL(82, 1710, 29), // "on_Dir_Offset_Y1_valueChanged"
QT_MOC_LITERAL(83, 1740, 29), // "on_Dir_Offset_Y2_valueChanged"
QT_MOC_LITERAL(84, 1770, 29), // "on_Dir_Offset_Y3_valueChanged"
QT_MOC_LITERAL(85, 1800, 29), // "on_Dir_Offset_Y4_valueChanged"
QT_MOC_LITERAL(86, 1830, 29), // "on_Dir_Offset_Y5_valueChanged"
QT_MOC_LITERAL(87, 1860, 29), // "on_Dir_Offset_Y6_valueChanged"
QT_MOC_LITERAL(88, 1890, 27) // "on_Rec_CameraHeight_clicked"

    },
    "CCD_SysParEditor\0slotSure\0\0slotCancel\0"
    "slotTurnToCamera\0slotTurnToCCDHeight\0"
    "slotTurnToCCDMag\0slotTurnToCCDOffset\0"
    "slotTurnToAuto\0slotGetMark\0index\0"
    "slotAutoLearnIdentify\0activeMainScreen\0"
    "slotCameraGain\0slotCameraShutter\0"
    "slotCameraContrast\0slotSetCameraMirrorV\0"
    "slotSetCameraMirrorH\0slotSetCameraHeight\0"
    "slotReadCameraH\0slotFetchImage\0"
    "slotShowMag\0slotCheckAll\0slotSetMagX\0"
    "slotSetMagY\0slotIdPos1\0slotIdPos2\0"
    "slotIdPos3\0slotIdPos4\0slotComputeMag\0"
    "slotSetOffsetX\0slotSetOffsetY\0"
    "slotSetOffsetAngle\0slotIdMarkForOffset\0"
    "slotGetMarkForOffset\0slotIdMarkForOffset1\0"
    "slotIdMarkForOffset2\0slotIdMarkForOffset3\0"
    "slotIdMarkForOffset4\0slotIdMarkForOffset5\0"
    "slotIdMarkForOffset6\0slotCalculateCenterPointAvg\0"
    "slotCalculateCenterPointAvgAuto\0"
    "slotCalculateCenterPoints\0slotCncAbort\0"
    "slotSaveMark\0log\0str\0on_Btn_GetPos_clicked\0"
    "on_Auto_PreMoveY_valueChanged\0arg1\0"
    "on_Auto_PreMoveA_valueChanged\0"
    "on_Auto_MinA_valueChanged\0"
    "on_Auto_MaxA_valueChanged\0"
    "on_Btn_AutoStart_clicked\0"
    "on_Btn_AutoReset_clicked\0"
    "on_Btn_GetAutoMark_clicked\0"
    "on_Auto_IdentifyDelay_valueChanged\0"
    "on_Auto_PlatformType_currentIndexChanged\0"
    "on_Btn_AutoIdentify_clicked\0"
    "on_Auto_PreMoveX_valueChanged\0"
    "on_Test_Num1_valueChanged\0"
    "on_Test_Num2_valueChanged\0"
    "on_Test_Num3_valueChanged\0"
    "on_Btn_EnableDirectPos_clicked\0"
    "on_Btn_EnableSmartPos_clicked\0"
    "on_Dir_Max_A_valueChanged\0"
    "on_Dir_Min_A_valueChanged\0"
    "on_Dir_Mag_X1_valueChanged\0"
    "on_Dir_Mag_X2_valueChanged\0"
    "on_Dir_Mag_Y2_valueChanged\0"
    "on_Dir_Mag_X4_valueChanged\0"
    "on_Dir_Mag_X3_valueChanged\0"
    "on_Dir_Mag_Y4_valueChanged\0"
    "on_Dir_Mag_Y3_valueChanged\0"
    "on_Dir_Mag_Y1_valueChanged\0"
    "on_Dir_Offset_X1_valueChanged\0"
    "on_Dir_Offset_X2_valueChanged\0"
    "on_Dir_Offset_X3_valueChanged\0"
    "on_Dir_Offset_X4_valueChanged\0"
    "on_Dir_Offset_X5_valueChanged\0"
    "on_Dir_Offset_X6_valueChanged\0"
    "on_Dir_Offset_Y1_valueChanged\0"
    "on_Dir_Offset_Y2_valueChanged\0"
    "on_Dir_Offset_Y3_valueChanged\0"
    "on_Dir_Offset_Y4_valueChanged\0"
    "on_Dir_Offset_Y5_valueChanged\0"
    "on_Dir_Offset_Y6_valueChanged\0"
    "on_Rec_CameraHeight_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCD_SysParEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      85,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  439,    2, 0x0a /* Public */,
       3,    0,  440,    2, 0x0a /* Public */,
       4,    0,  441,    2, 0x0a /* Public */,
       5,    0,  442,    2, 0x0a /* Public */,
       6,    0,  443,    2, 0x0a /* Public */,
       7,    0,  444,    2, 0x0a /* Public */,
       8,    0,  445,    2, 0x0a /* Public */,
       9,    1,  446,    2, 0x0a /* Public */,
       9,    0,  449,    2, 0x2a /* Public | MethodCloned */,
      11,    0,  450,    2, 0x0a /* Public */,
      12,    0,  451,    2, 0x0a /* Public */,
      13,    1,  452,    2, 0x0a /* Public */,
      14,    1,  455,    2, 0x0a /* Public */,
      15,    1,  458,    2, 0x0a /* Public */,
      16,    1,  461,    2, 0x0a /* Public */,
      17,    1,  464,    2, 0x0a /* Public */,
      18,    1,  467,    2, 0x0a /* Public */,
      19,    0,  470,    2, 0x0a /* Public */,
      20,    1,  471,    2, 0x0a /* Public */,
      21,    0,  474,    2, 0x0a /* Public */,
      22,    1,  475,    2, 0x0a /* Public */,
      23,    1,  478,    2, 0x0a /* Public */,
      24,    1,  481,    2, 0x0a /* Public */,
      25,    0,  484,    2, 0x0a /* Public */,
      26,    0,  485,    2, 0x0a /* Public */,
      27,    0,  486,    2, 0x0a /* Public */,
      28,    0,  487,    2, 0x0a /* Public */,
      29,    0,  488,    2, 0x0a /* Public */,
      30,    1,  489,    2, 0x0a /* Public */,
      31,    1,  492,    2, 0x0a /* Public */,
      32,    1,  495,    2, 0x0a /* Public */,
      33,    1,  498,    2, 0x0a /* Public */,
      34,    0,  501,    2, 0x0a /* Public */,
      35,    0,  502,    2, 0x0a /* Public */,
      36,    0,  503,    2, 0x0a /* Public */,
      37,    0,  504,    2, 0x0a /* Public */,
      38,    0,  505,    2, 0x0a /* Public */,
      39,    0,  506,    2, 0x0a /* Public */,
      40,    0,  507,    2, 0x0a /* Public */,
      41,    0,  508,    2, 0x0a /* Public */,
      42,    0,  509,    2, 0x0a /* Public */,
      43,    0,  510,    2, 0x0a /* Public */,
      44,    0,  511,    2, 0x0a /* Public */,
      45,    0,  512,    2, 0x0a /* Public */,
      46,    1,  513,    2, 0x0a /* Public */,
      48,    0,  516,    2, 0x08 /* Private */,
      49,    1,  517,    2, 0x08 /* Private */,
      51,    1,  520,    2, 0x08 /* Private */,
      52,    1,  523,    2, 0x08 /* Private */,
      53,    1,  526,    2, 0x08 /* Private */,
      54,    0,  529,    2, 0x08 /* Private */,
      55,    0,  530,    2, 0x08 /* Private */,
      56,    0,  531,    2, 0x08 /* Private */,
      57,    1,  532,    2, 0x08 /* Private */,
      58,    1,  535,    2, 0x08 /* Private */,
      59,    0,  538,    2, 0x08 /* Private */,
      60,    1,  539,    2, 0x08 /* Private */,
      61,    1,  542,    2, 0x08 /* Private */,
      62,    1,  545,    2, 0x08 /* Private */,
      63,    1,  548,    2, 0x08 /* Private */,
      64,    0,  551,    2, 0x08 /* Private */,
      65,    0,  552,    2, 0x08 /* Private */,
      66,    1,  553,    2, 0x08 /* Private */,
      67,    1,  556,    2, 0x08 /* Private */,
      68,    1,  559,    2, 0x08 /* Private */,
      69,    1,  562,    2, 0x08 /* Private */,
      70,    1,  565,    2, 0x08 /* Private */,
      71,    1,  568,    2, 0x08 /* Private */,
      72,    1,  571,    2, 0x08 /* Private */,
      73,    1,  574,    2, 0x08 /* Private */,
      74,    1,  577,    2, 0x08 /* Private */,
      75,    1,  580,    2, 0x08 /* Private */,
      76,    1,  583,    2, 0x08 /* Private */,
      77,    1,  586,    2, 0x08 /* Private */,
      78,    1,  589,    2, 0x08 /* Private */,
      79,    1,  592,    2, 0x08 /* Private */,
      80,    1,  595,    2, 0x08 /* Private */,
      81,    1,  598,    2, 0x08 /* Private */,
      82,    1,  601,    2, 0x08 /* Private */,
      83,    1,  604,    2, 0x08 /* Private */,
      84,    1,  607,    2, 0x08 /* Private */,
      85,    1,  610,    2, 0x08 /* Private */,
      86,    1,  613,    2, 0x08 /* Private */,
      87,    1,  616,    2, 0x08 /* Private */,
      88,    0,  619,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,   10,
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
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void, QMetaType::Double,   50,
    QMetaType::Void,

       0        // eod
};

void CCD_SysParEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCD_SysParEditor *_t = static_cast<CCD_SysParEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSure(); break;
        case 1: _t->slotCancel(); break;
        case 2: _t->slotTurnToCamera(); break;
        case 3: _t->slotTurnToCCDHeight(); break;
        case 4: _t->slotTurnToCCDMag(); break;
        case 5: _t->slotTurnToCCDOffset(); break;
        case 6: _t->slotTurnToAuto(); break;
        case 7: _t->slotGetMark((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slotGetMark(); break;
        case 9: _t->slotAutoLearnIdentify(); break;
        case 10: _t->activeMainScreen(); break;
        case 11: _t->slotCameraGain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->slotCameraShutter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slotCameraContrast((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->slotSetCameraMirrorV((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->slotSetCameraMirrorH((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->slotSetCameraHeight((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 17: _t->slotReadCameraH(); break;
        case 18: _t->slotFetchImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->slotShowMag(); break;
        case 20: _t->slotCheckAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->slotSetMagX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 22: _t->slotSetMagY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 23: { bool _r = _t->slotIdPos1();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: { bool _r = _t->slotIdPos2();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: { bool _r = _t->slotIdPos3();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 26: { bool _r = _t->slotIdPos4();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 27: _t->slotComputeMag(); break;
        case 28: _t->slotSetOffsetX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 29: _t->slotSetOffsetY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 30: _t->slotSetOffsetAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 31: _t->slotIdMarkForOffset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->slotGetMarkForOffset(); break;
        case 33: _t->slotIdMarkForOffset1(); break;
        case 34: _t->slotIdMarkForOffset2(); break;
        case 35: _t->slotIdMarkForOffset3(); break;
        case 36: _t->slotIdMarkForOffset4(); break;
        case 37: _t->slotIdMarkForOffset5(); break;
        case 38: _t->slotIdMarkForOffset6(); break;
        case 39: _t->slotCalculateCenterPointAvg(); break;
        case 40: _t->slotCalculateCenterPointAvgAuto(); break;
        case 41: _t->slotCalculateCenterPoints(); break;
        case 42: _t->slotCncAbort(); break;
        case 43: _t->slotSaveMark(); break;
        case 44: _t->log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 45: _t->on_Btn_GetPos_clicked(); break;
        case 46: _t->on_Auto_PreMoveY_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 47: _t->on_Auto_PreMoveA_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 48: _t->on_Auto_MinA_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 49: _t->on_Auto_MaxA_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 50: _t->on_Btn_AutoStart_clicked(); break;
        case 51: _t->on_Btn_AutoReset_clicked(); break;
        case 52: _t->on_Btn_GetAutoMark_clicked(); break;
        case 53: _t->on_Auto_IdentifyDelay_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 54: _t->on_Auto_PlatformType_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 55: _t->on_Btn_AutoIdentify_clicked(); break;
        case 56: _t->on_Auto_PreMoveX_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 57: _t->on_Test_Num1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 58: _t->on_Test_Num2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 59: _t->on_Test_Num3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 60: _t->on_Btn_EnableDirectPos_clicked(); break;
        case 61: _t->on_Btn_EnableSmartPos_clicked(); break;
        case 62: _t->on_Dir_Max_A_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 63: _t->on_Dir_Min_A_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 64: _t->on_Dir_Mag_X1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 65: _t->on_Dir_Mag_X2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 66: _t->on_Dir_Mag_Y2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 67: _t->on_Dir_Mag_X4_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 68: _t->on_Dir_Mag_X3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 69: _t->on_Dir_Mag_Y4_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 70: _t->on_Dir_Mag_Y3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 71: _t->on_Dir_Mag_Y1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 72: _t->on_Dir_Offset_X1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 73: _t->on_Dir_Offset_X2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 74: _t->on_Dir_Offset_X3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 75: _t->on_Dir_Offset_X4_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 76: _t->on_Dir_Offset_X5_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 77: _t->on_Dir_Offset_X6_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 78: _t->on_Dir_Offset_Y1_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 79: _t->on_Dir_Offset_Y2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 80: _t->on_Dir_Offset_Y3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 81: _t->on_Dir_Offset_Y4_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 82: _t->on_Dir_Offset_Y5_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 83: _t->on_Dir_Offset_Y6_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 84: _t->on_Rec_CameraHeight_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CCD_SysParEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CCD_SysParEditor.data,
      qt_meta_data_CCD_SysParEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCD_SysParEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCD_SysParEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCD_SysParEditor.stringdata0))
        return static_cast<void*>(const_cast< CCD_SysParEditor*>(this));
    if (!strcmp(_clname, "CCD_MarksFace"))
        return static_cast< CCD_MarksFace*>(const_cast< CCD_SysParEditor*>(this));
    if (!strcmp(_clname, "CNC_Listener"))
        return static_cast< CNC_Listener*>(const_cast< CCD_SysParEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int CCD_SysParEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 85)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 85;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 85)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 85;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
