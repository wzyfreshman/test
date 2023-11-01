#ifndef RS_SPLINE_H
#define RS_SPLINE_H

#include <QList>
#include "rs_vector.h"
#include "rs_entity.h"
#include "rs_atomicentity.h"

/**
 * Holds the data that defines a line.
 */
class CCD_API RS_SplineData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_SplineData() {}

        RS_SplineData(int degree, bool closed) {
                this->degree = degree;
                this->closed = closed;
        }

    friend std::ostream& operator << (std::ostream& os, const RS_SplineData& ld) {
        os << "( degree: " << ld.degree <<
        " closed: " << ld.closed <<
        ")";
        return os;
    }

public:
        /** Degree of the spline (1, 2, 3) */
        int degree;
        /** Closed flag. */
        bool closed;
        /** Control points of the spline. */
    QList<RS_Vector> controlPoints;

};


class CCD_API RS_Spline : public RS_AtomicEntity {
public:
    RS_Spline( const RS_SplineData& d);
    virtual RS_Entity* clone();
    virtual ~RS_Spline();
    virtual RS2::EntityType getEntityType() const {
        return RS2::EntitySpline;
    }
    virtual bool isEdge() const {
        return false;
    }

    RS_SplineData getData() const {
        return data;
    }

        void setDegree(int deg) {
                if (deg>=1 && deg<=3) {
                        data.degree = deg;
                }
        }
        int getDegree() {
                return data.degree;
        }

    int getNumberOfKnots() {
                return 0;
        }
        int getNumberOfControlPoints() {
                return data.controlPoints.count();
        }

        bool isClosed() {
                return data.closed;
        }

        void setClosed(bool c) {
                data.closed = c;
                update();
        }
    virtual RS_VectorSolutions getRefPoints();
    virtual RS_Vector getStartpoint() const ;
    virtual RS_Vector getEndpoint() const ;
     void update(int seg = 8);

    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL)const;
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL,
                                       int middlePoints = 1)const;
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
        virtual void addControlPoint(const RS_Vector& v);
        virtual void removeLastControlPoint();
        virtual void scale(const RS_Vector& center, const RS_Vector& factor);
        virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);
        QList<RS_Vector> getControlPoints();
        friend std::ostream& operator << (std::ostream& os, const RS_Spline& l);
        virtual void calculateBorders();
        static void rbasis(int c, double t, int npts, int x[], double h[], double r[]);
        static void knot(int num, int order, int knotVector[]);
        static void rbspline(int npts, int k, int p1,
                             double b[], double h[], double p[]);
        static void knotu(int num, int order, int knotVector[]);
        static void rbsplinu(int npts, int k, int p1,
                             double b[], double h[], double p[]);

        QList<RS_Entity *> local_entities;
        RS_Vector minV;
        RS_Vector maxV;
protected:
        void addEntity(RS_Entity *);
        void clear();
        RS_SplineData data;


};

#endif
