#ifndef CD_PARAMETER_H
#define CD_PARAMETER_H

#include <stdlib.h>

#include "cd_inisettings.h"

class CCD_API CD_Parameter
{

    friend class CD_FeatureAngle;
    friend class CD_FeatureCross;
    friend class CD_FeatureCircle;
    friend class CD_FeatureRelate;
    friend class CD_OperateFeature;
    friend class CCD_SysParCommonEditor;
    friend class RS_MathAcc;

public:
    CD_Parameter();
    static CD_Parameter *Instance();

    void saveToDisk( CD_IniSettings &set);
    void readFromDisk( CD_IniSettings &set);
    static void commonSettingSaveToDisk( CD_IniSettings &set);
    static void commonSettingReadFromDisk( CD_IniSettings &set);

    int getErosionSize();
    int getCannyThresholdH();
    int getCannyThresholdL();
    int getMglMinR();

    int getDetectArcLen() const {return detectarclen;}
    double getDetectArcMaxR() const {return detectarcmaxr;}
    double getIsArcTolerance() const {return isarctolerance;}
    double getDetectArcCenterTolerance() const {return detectarccentertolerance;}
    double getDetectArcRadiusTolerance() const {return detectarcradiustolerance;}
    double getIdentifyArcRadiusTolerance() const {return identifyarcradiustolerance;}
    double getMatchFactor() const {return matchfactor;}
    int getSearchLineWidth() const {return searchlinewidth;}
    int getMergeLineWidth() const {return mergelinewidth;}

    double getAngleTolerance();
    double getParallelTolerance();
    double getParallelWidthTolerance();
    double getSimilarTolerance();
    double getSimilarToleranceR();
    double getSimilarToleranceThr();
    int findLineMinLenght();
    int getMaxRotate();
    int getContrastValidValue();


    bool isDebugMode();
    void enableDebugMode();
    void disableDebugMode();

    void debugPrint();

public:

private:


    //*all to get edege
    int erosionsize;

    //*cross and angle
    int findlineminlenght;
    int searchlinewidth;
    int mergelinewidth;

    //*arc
    int detectarclen;
    double detectarccentertolerance;
    double detectarcradiustolerance;
    double detectarcmaxr;

    static double identifyarcradiustolerance;
    static double isarctolerance;

    //*find match factor
    static double matchfactor;

    //*cross and arc
    static double angletolerance; //zhi jiao rong cha
    static double paralleltolerance;
    static double parallelwidthtolerance;
    static double similartolerance;


    static int contrast_valid_value;
    static int radd_vvd;


    //*current do not use yet
    static int cannythresholdl;
    static int cannythresholdh;
    static int mergelineminr;
    static double similartolerancer;
    static double similartolerancethr;

    static int maxmarkrotate;
    static bool debugmode;
    static CD_Parameter *instance;

};

#endif // CD_PARAMETER_H
