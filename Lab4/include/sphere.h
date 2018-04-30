
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "containable.h"
#include "point.h"
#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"
#include "cube.h"
#include "reuleauxtetrahedron.h"

class Sphere : public Containable3D {
    private:
        Point3D center_;
        double radius_;

    public:
        //Circle(const Point3D &center, double radius);
        Sphere(Point3D center, double radius);

        Point3D center();
        void setCenter(const Point3D &center);

        double radius();
        void setRadius(double radius);

        bool containedWithin(Sphere &sphere);
        bool containedWithin(Cube &cube);
        bool containedWithin(ReuleauxTetrahedron &rt);
};

#endif
