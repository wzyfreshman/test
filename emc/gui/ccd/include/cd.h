#ifndef CD_H
#define CD_H

#include <QDebug>
#include <QObject>

#include "ccd_setting.h"

class CCD_API CD {
public:
    enum MarkType {
        NoneMark,
        BaseMark,
        CrossMark,
        AngleMark,
        CircleMark,
        RelateMark,
        InterestMark
    };

    enum FeatureType {
        FeatureNone,
        FeatureCross,
        FeatureAngle,
        FeatureCircle,
        FeatureRelate
    };

    enum MarkPosition {

        Position1,
        Position2,
        Position3,
        Position4

    };

    enum OperMode {
        OperAuto,
        OperManual
    };

    enum AnglePos {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

};
#endif // CD_H
