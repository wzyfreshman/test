#ifndef RS_LINE_H
#define RS_LINE_H

#include "rs_vector.h"
#include "rs_math.h"
#include "rs_entity.h"
#include "rs_atomicentity.h"
#include "lc_quadratic.h"

#include <QPoint>

class CCD_API RS_LineData {

public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_LineData() {}

    RS_LineData(const RS_Vector& startpoint,
                const RS_Vector& endpoint) {

        this->startpoint = startpoint;
        this->endpoint = endpoint;
    }

    RS_LineData(double r,double angle) {
        this->r = r;
        this->angle = angle;
    }

    friend class RS_Line;

    friend std::ostream& operator << (std::ostream& os, const RS_LineData& ld) {
        os << "(" << ld.startpoint <<
           "/" << ld.endpoint <<
           ")";
        return os;
    }

public:
    RS_Vector startpoint;
    RS_Vector endpoint;
    double r;
    double angle;

};


/**
 * Class for a line entity.
 *
 * @author Andrew Mustun
 */
class CCD_API RS_Line :public RS_AtomicEntity {
public:

    RS_Line();
    RS_Line(const RS_LineData& d);
    RS_Line(double r,double angle);
    RS_Line(const RS_Vector& pStart,
            const RS_Vector& pEnd);

    virtual ~RS_Line();
    virtual RS_Entity* clone();
    virtual RS2::EntityType getEntityType() {
        return RS2::EntityLine;
    }
    /** @return Copy of data that defines the line. */
    RS_LineData getData() const {
        return data;
    }
    bool isInfinite() const {
        return infinite;
    }
    double getR() const {
        return data.r;
    }
    double getThr() const {
        return data.angle;
    }

    QList<QPoint> getPoints() const{
        return points;
    }

    void setR(double set)  {
         data.r = set;
    }
    void setThr(double set)  {
         data.angle = set;
    }
    void setPoints(const QList<QPoint> &set) {
            points = set;
    }
    /** @return Start point of the entity */
    virtual RS_Vector getStartpoint() const {
        return data.startpoint;
    }
    /** @return End point of the entity */
    virtual RS_Vector getEndpoint() const {
        return data.endpoint;
    }
    /** Sets the startpoint */
    void setStartpoint(RS_Vector s) {
        data.startpoint = s;
        calculateBorders();
    }
    /** Sets the endpoint */
    void setEndpoint(RS_Vector e) {
        data.endpoint = e;
        calculateBorders();
    }
    /**
     * @return Direction 1. The angle at which the line starts at
     * the startpoint.
     */
    double getDirection1() const {
        return getAngle1();
    }
    /**
     * @return Direction 2. The angle at which the line starts at
     * the endpoint.
     */
    double getDirection2() const {
        return getAngle2();
    }
    virtual RS_Vector getTangentDirection(const RS_Vector& point)const;

    virtual RS_Vector getNearestCenter(const RS_Vector &, double *);
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity=true, double* dist = NULL, RS_Entity** entity=NULL)const;
    virtual void moveStartpoint(const RS_Vector& pos);
    virtual void moveEndpoint(const RS_Vector& pos);
    virtual RS2::Ending getTrimPoint(const RS_Vector& trimCoord,
                                     const RS_Vector& trimPoint);
    virtual void reverse();

    //virtual LC_Quadratic getQuadratic() const;

    virtual double getDistanceToPoint(const RS_Vector &coord, RS_Entity **entity,
                                      RS2::ResolveLevel level, double solidDist) const;

    /** Sets the y coordinate of the startpoint */
    void setStartpointY(double val) {
        data.startpoint.y = val;
        calculateBorders();
    }
    /** Sets the y coordinate of the endpoint */
    void setEndpointY(double val) {
        data.endpoint.y = val;
        calculateBorders();
    }
    virtual bool hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2);

    /**
     * @return The length of the line.
     */
    virtual double getLength() const {
        return data.startpoint.distanceTo(data.endpoint);
    }

    /**
     * @return The angle of the line (from start to endpoint).
     */
    virtual double getAngle1() const {
        return data.startpoint.angleTo(data.endpoint);
    }

    /**
     * @return The angle of the line (from end to startpoint).
     */
    virtual double getAngle2() const {
        return data.endpoint.angleTo(data.startpoint);
    }
    //virtual bool isTangent(const RS_CircleData&  circleData);

    RS_Vector getNormalVector() const;
    virtual RS_Vector getMiddlePoint()const;
    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL)const;

    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);

    virtual RS_Vector getNearestDist(double distance,
                                     bool startp);
    //virtual RS_Vector getNearestRef(const RS_Vector& coord,
    //                                 double* dist = NULL);
    //virtual double getDistanceToPoint(const RS_Vector& coord,
                                      //RS_Entity** entity=NULL,
     //                                 RS2::ResolveLevel level=RS2::ResolveNone,
     //                                 double solidDist = RS_MAXDOUBLE)const;
    /**
          * implementations must revert the direction of an atomic entity
          */
    virtual void revertDirection();
    // virtual QVector<RS_Entity* > offsetTwoSides(const double& distance) const;


    virtual bool offset(const RS_Vector& coord, const double& distance);
    virtual void move(const RS_Vector& offset);
    virtual void rotate(const double& angle);
    virtual void rotate(const RS_Vector& center, const double& angle);
    virtual void rotate(const RS_Vector& center, const RS_Vector& angleVector);
    virtual void scale(const RS_Vector& factor);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

    virtual double distanceToPoint(const RS_Vector &);

    bool between(double a, double X0, double X1);
    //bool detectIntersect(const RS_Line &line,RS_Vector &cossP);

    /** whether the entity's bounding box intersects with visible portion of graphic view */
//    virtual bool isVisibleInWindow(RS_GraphicView* view) const;
    //virtual void draw(RS_Painter* painter, RS_GraphicView* view, double& patternOffset);
    //virtual void drawArrow(RS_Painter* painter,RS_GraphicView* view,const RS_Vector &start,const RS_Vector &end);
    friend std::ostream& operator << (std::ostream& os, const RS_Line& l);

    double angleToVertical();
    virtual void calculateBorders();
    void calculateRandThr();
    bool calculateRandThrFromAbc();
    void calculateLineAbcFromThr();
    void calculateLineAbcFromTwoPoints();
    double angleToLine(RS_Line &line);
    QList<QPoint> fatherlink;
    QList<int> fatherlinknum;
    bool forCalculate;
    double a,b,c;
protected:
    RS_LineData data;
    RS_Vector minV;
    RS_Vector maxV;
    QList<QPoint> points;
    bool haspoints;
    bool infinite;

};

#endif
