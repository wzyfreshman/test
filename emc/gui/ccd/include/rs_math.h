#ifndef RS_MATH_H
#define RS_MATH_H

// no idea why, but doesn't link without that under win32 / bcc55:
#ifndef _MT
#define _MT
#endif

#include <cfloat>
#include <cmath>
#include <complex>
#include <errno.h>
#include <cstdlib>


#include <QRegExp>
#include <QVector>

#ifdef EMU_C99
#include "emu_c99.h"
#endif

#include "rs_vector.h"


#define ARAD 57.29577951308232

//tolerance
#define RS_TOLERANCE 1.0e-10


//squared tolerance
#define RS_TOLERANCE15 1.5e-15
#define RS_TOLERANCE2 1.0e-20
#define RS_TOLERANCE_ANGLE 1.0e-8


class CD_Parameter;
class RS_Line;

/**
 * Math functions.
 */

class CCD_API RS_Math {
public:

    static double pow(double x, double y);
    static RS_Vector pow(RS_Vector x, double y);

    static bool passPointParallelsLine(const RS_Vector &p1, const RS_Vector &p2,
                                         double apattern, double bpattern, double *aget, double *bget, double *cget) ;
    static bool lineBisector(const RS_Vector &p1,const RS_Vector &p2,
                             double *a,double *b,double *c);
    static RS_Vector crossPointBtwTwoLines(double a1,double b1,double c1,
                                           double a2,double b2,double c2);

    static void twoPointToLine(const RS_Vector &p1,const RS_Vector &p2,double *ap,double *bp,double *cp);
    static double distanceFromPointToLine(const RS_Vector &p,double a,double b,double c);
    static double similarBtwTwoLines(RS_Line *p1,RS_Line *p2);
    static bool isInToleranceBtwTwoLines(RS_Line *p1,RS_Line *p2,CD_Parameter *p);
    static RS_Vector crossPointBtwTwoLines(RS_Line *p1,RS_Line *p2);
    static RS_Vector crossPointBtwTwoLines(const RS_Line &p1,const RS_Line &p2);

    static int checkParallelBtwTwoLines(RS_Line *p1,RS_Line *p2,CD_Parameter *p);
    static int checkParallelBtwTwoLines(const RS_Line &p1,const RS_Line &p2,CD_Parameter &p);
    static bool checkAngleBtwTwoLines(RS_Line *p1,RS_Line *p2,CD_Parameter *p);
    static bool checkAngleBtwTwoLines(const RS_Line &p1,const RS_Line &p2,CD_Parameter &p);

    static void correctTwoParaLines(RS_Line *p1,RS_Line *p2);
    static void correctTwoCrossLines(RS_Line *p1,RS_Line *p2);


    static double rad2deg(double a);
    static double deg2rad(double a);
    static double rad2gra(double a);
    static int findGCD(int a, int b);
    static bool isAngleBetween(double a,
                               double a1, double a2,
                               bool reversed);
    static double correctAngle(double a);
    static double getAngleDifference(double a1, double a2, bool reversed = false);
    static double makeAngleReadable(double angle, bool readable=true,
                                    bool* corrected=NULL);
    static bool isAngleReadable(double angle);
    static bool isSameDirection(double dir1, double dir2, double tol);
    //static double eval(const QString& expr, double def=0.0);

    static bool cmpDouble(double v1, double v2, double tol=0.001);
    static double cmpAngle(double v1, double v2, double tol=0.001);


    static double angleDiff( double x2, double x1);
    static double angleAvg(double *angle, int n);

//swap of two variables
    template <class T>
    static void swap( T &a, T &b) {
        const T ttmp(a);
        a=b;
        b=ttmp;
    }

    static QString doubleToString(double value, double prec);
    static QString doubleToString(double value, int prec);

    static void test();
};

#endif
