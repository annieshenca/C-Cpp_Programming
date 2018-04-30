
#ifndef _CUBE_H_
#define _CUBE_H_

#include <vector>

#include "containable.h"
#include "point.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"
#include "sphere.h"
#include "reuleauxtetrahedron.h"


class Cube : public Containable3D {
    private:
        std::vector<Point3D> vertices_;

    public:
        // RegularConvexPolygon(std::vector<Point3D> vertices);
        Cube(Point3D upperFace[4], Point3D lowerFace[4]);

        std::vector<Point3D> vertices();
        std::vector<Line> edges();
        int vertexCount();

        bool containedWithin(Sphere &sphere);
        bool containedWithin(Cube &cube);
        bool containedWithin(ReuleauxTetrahedron &rt);
};

#endif
