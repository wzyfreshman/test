#ifndef RS_CIRCLE_H
#define RS_CIRCLE_H

#define RS_TOLERANCE_CIRCLER 0.01

#include "rs_vector.h"
#include "rs_math.h"
#include "rs_entity.h"
#include "rs_atomicentity.h"

#include <math.h>

class CCD_API RS_CircleData {
public:
    RS_CircleData() {}

    RS_CircleData(const RS_Vector& center,
                  double radius) {

        this->center = center;
        this->radius = radius;
    }

    RS_CircleData(double x,double y,
                  double radius)
        :center(x,y){

        this->radius = radius;
    }

    void reset() {
        center = RS_Vector(false);
        radius = 0.0;
    }

    bool isValid() {
        return (center.valid && radius>RS_TOLERANCE_CIRCLER);
    }

    friend class RS_Circle;

    friend std::ostream& operator << (std::ostream& os,
                                      const RS_CircleData& ad) {
        os << "(" << ad.center <<
              "/" << ad.radius <<
              ")";
        return os;
    }

public:
    RS_Vector center;
    double radius;

};


class CCD_API RS_Circle : public RS_AtomicEntity {

public:
    RS_Circle (const RS_CircleData& d);
    RS_Circle (const RS_Vector& v,const double r);
    RS_Circle (const double x=0,const double y=0,const double r=0);

    virtual ~RS_Circle();
    virtual RS_Entity* clone();

    virtual RS2::EntityType getEntityType() {
        return RS2::EntityCircle;
    }

    RS_CircleData getData() const {
        return data;
    }


    double getDirection1() const {
        return M_PI/2.0;
    }
    /**
         * @return Direction 2. The angle at which the arc starts at
         * the endpoint.
         */
    double getDirection2() const {
        return M_PI/2.0*3.0;
    }

    /** @return The center point (x) of this arc */
    virtual RS_Vector getCenter() const {
        return data.center;
    }
    /** Sets new center. */
    void setCenter(const RS_Vector& c) {
        data.center = c;
    }
    /** @return The radius of this arc */
    virtual double getRadius() const {
        return data.radius;
    }
    /** Sets new radius. */
    void setRadius(double r) {
        data.radius = r;
    }
    double getAngleLength() const;
    virtual double getLength() const;
    virtual bool isTangent(const RS_CircleData&  circleData);

    bool createFromCR(const RS_Vector& c, double r);
    bool createFrom2P(const RS_Vector& p1, const RS_Vector& p2);
    bool createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                      const RS_Vector& p3);
    bool createFrom3P(const RS_VectorSolutions& sol);

    virtual RS_Vector getMiddlePoint(void)const;
    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL)const;
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
                                              bool onEntity = true, double* dist = NULL, RS_Entity** entity=NULL)const;
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL,
                                       int middlePoints = 1
            )const;
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     bool startp);
    virtual RS_Vector getNearestOrthTan(const RS_Vector& coord,
                                        const RS_Line& normal,
                                        bool onEntity = false);

    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity=NULL,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE) const;

    virtual bool offset(const RS_Vector& coord, const double& distance);
    virtual RS_VectorSolutions getTangentPoint(const RS_Vector& point) const;//find the tangential points seeing from given point
    virtual RS_Vector getTangentDirection(const RS_Vector& point)const;
    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, const double& angle);
    virtual void rotate(const RS_Vector& center, const RS_Vector& angleVector);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);



    friend std::ostream& operator << (std::ostream& os, const RS_Circle& a);



protected:
    RS_CircleData data;
};


#endif // RS_CIRCLE_H
