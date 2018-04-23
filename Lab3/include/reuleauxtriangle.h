/*
 * Annie Shen (ashen7)
 * CMPS 109 - Spring 2018
 * Lab 3 - Due Apr 22, 2018
 */

#ifndef _REULEAUXTRIANGLE_H_
#define _REULEAUXTRIANGLE_H_

#include <vector>
#include "containable.h"
#include "point.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"

class ReuleauxTriangle : public Containable2D {
    private:
        std::vector<Point2D> vertices_;

    public:
        ReuleauxTriangle(std::vector<Point2D> vertices);

        std::vector<Point2D> vertices();
        std::vector<Line> edges();
        //std::vector<Circle>???;

        bool containedWithin(Circle &circle);
        bool containedWithin(RegularConvexPolygon &polygon);
        bool containedWithin(ReuleauxTriangle &rt);
};

#endif
