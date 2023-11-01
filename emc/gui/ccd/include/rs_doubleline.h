#ifndef RS_DOUBLELINE_H
#define RS_DOUBLELINE_H

#include "rs_line.h"
#include "cd_parameter.h"

class CCD_API RS_DoubleLine {

public:
    RS_DoubleLine();
    RS_DoubleLine(const RS_Line &l1, const RS_Line &l2,const CD_Parameter &p);
    RS_DoubleLine(const RS_Line &l1, const RS_Line &l2);
    double getR();
    double getThr();
    double getDistance();
    RS_Line *getLine1();
    RS_Line *getLine2();
    RS_Line getMidleLine() const;
private:
    RS_Line line1;
    RS_Line line2;
    RS_Line midle;
    CD_Parameter par;
};

#endif // RS_DOUBLELINE_H
