#ifndef RS_ENTITY_H
#define RS_ENTITY_H

#include "rs_vector.h"
#include "lc_quadratic.h"
#include "rs_math.h"

#include "ccd_setting.h"


class RS {

public:
    /**
     * Entity types returned by the rtti() method
     */
    enum EntityType {
        EntityNone,      /**< Unknown */
        EntityContainer,    /**< Container */
        EntityBlock,        /**< Block (Group definition) */
        EntityFontChar,     /**< Font character */
        EntityInsert,       /**< Insert (Group instance) */
        EntityGraphic,      /**< Graphic with layers */
        EntityPoint,        /**< Point */
        EntityLine,         /**< Line */
        EntityPolyline,     /**< Polyline */
        EntityVertex,       /**< Vertex (part of a polyline) */
        EntityArc,          /**< Arc */
        EntityCircle,       /**< Circle */
        EntityEllipse,      /**< Ellipse */
        EntityHyperbola,      /**< Hyperbola */
        EntitySolid,        /**< Solid */
        EntityConstructionLine, /**< Construction line */
        EntityMText,         /**< Multi-line Text */
        EntityText,         /**< Single-line Text */
        EntityDimAligned,   /**< Aligned Dimension */
        EntityDimLinear,    /**< Linear Dimension */
        EntityDimRadial,    /**< Radial Dimension */
        EntityDimDiametric, /**< Diametric Dimension */
        EntityDimAngular,   /**< Angular Dimension */
        EntityDimLeader,    /**< Leader Dimension */
        EntityHatch,        /**< Hatch */
        EntityImage,        /**< Image */
        EntitySpline,       /**< Spline */
        EntityOverlayBox,    /**< OverlayBox */
        EntityPreview    /**< Preview Container */
    };

};

class RS_Entity
{

public:
    RS_Entity();
    virtual ~RS_Entity();
    virtual RS2::EntityType getEntityType();
    virtual RS_Entity* clone();
    virtual bool isAtomic() {return true;}

    virtual RS_Vector getEndpoint() {return RS_Vector(false);}
    virtual RS_Vector getStartpoint() {return RS_Vector(false);}

    virtual RS_Vector getNearestEndpoint(const RS_Vector&,
                                         double*  = NULL) {return RS_Vector(false);}
    virtual RS_Vector getNearestCenter(const RS_Vector &, double *) {return RS_Vector(false);}

    virtual void revertDirection() {}
    virtual bool offset(const RS_Vector&, const double&) {return false;}

    virtual RS_Vector getMiddlePoint() {return RS_Vector(false);}
    virtual double getDistanceToPoint(const RS_Vector& ,
                                      RS_Entity**  = NULL,
                                      RS2::ResolveLevel  = RS2::ResolveNone,
                                      double  = RS_MAXDOUBLE) {return 0.0;}

    virtual void move(const RS_Vector&) {}
    virtual void rotate(const RS_Vector& , const double& ) {}
    virtual void rotate(const RS_Vector& , const RS_Vector& ){}
    virtual void scale(const RS_Vector&, const RS_Vector& ) {}

    virtual double getDirection2() {
        return 0.0;
    }
    virtual double getDirection1() {
        return 0.0;
    }

    bool isContainer() {return false;}
    unsigned long int getId() const {
        return id;
    }

    virtual bool isPointOnEntity(const RS_Vector& coord,
                                 double tolerance=RS_TOLERANCE)  {

        RS_Vector v = coord;
        double dist = getDistanceToPoint(v, NULL, RS2::ResolveNone);
        return (dist<=fabs(tolerance));

    };

    virtual RS_Vector getTangentDirection(const RS_Vector& ) const {return RS_Vector(false);}


protected:

    unsigned long int id;
};

#endif // RS_ENTITY_H
