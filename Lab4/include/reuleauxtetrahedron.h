
#ifndef _REULEAUXTRIANGLE_H_
#define _REULEAUXTRIANGLE_H_

#include <vector>
#include "containable.h"
#include "point.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"
#include "sphere.h"
#include "cube.h"

class ReuleauxTetrahedron : public Containable3D {
    private:
        std::vector<Point3D> ver_;

    public:
        //ReuleauxTriangle(std::vector<Point2D> vertices);
        ReuleauxTetrahedron(Point3D vertices[4]);

        // std::vector<Point2D> vertices();
        // std::vector<Line> edges();
        // std::vector<Circle>???;

        bool containedWithin(Sphere &sphere);
        bool containedWithin(Cube &cube);
        bool containedWithin(ReuleauxTetrahedron &rt);
};

#endif
