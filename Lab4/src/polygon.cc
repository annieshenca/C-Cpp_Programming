/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include "polygon.h"
#include "geom.h"
#include "reuleauxtriangle.h"

RegularConvexPolygon::RegularConvexPolygon(std::vector<Point2D> vertices) {
    vertices_ = vertices;
}

std::vector<Point2D> RegularConvexPolygon::vertices() {
    return vertices_;
}

std::vector<Line> RegularConvexPolygon::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++)
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}

int RegularConvexPolygon::vertexCount() {
    return vertices_.size();
}

/*
 * Polygon in C
 */
bool RegularConvexPolygon::containedWithin(Circle &circle) {
    // If any of the vertices are outside the circle.
    for (Point2D const &vertex : vertices()) {
        // Imported geom.h to be able to use it here.
        if ( Geom::distance(vertex, circle.center()) > circle.radius() )
            return false;
    }
    return true;
}


/*
 * If any poly edge intersects with any polygon edge, poly is not contained.
 * If line between centers intersects with any polygon edge, poly is not contained
 * Only other possibility is that the poly is contained
 */
bool RegularConvexPolygon::containedWithin(RegularConvexPolygon &polygon) {

    Line line(Geom::center(polygon), Geom::center(*this)); // Centers of polygon
    for (Line const &edge : polygon.edges()) {
        if (Geom::intersects(edge, line))
            return false;
    }

    for (Line const &edge : polygon.edges()) {
        for (Line const &inner : edges()) {
            if (Geom::intersects(edge, inner)) {
                //printf("lines intersected!!!\n");
                return false;
            }
        }
    }
    return true;
}

/*
 * If any poly edge intersects with any polygon edge, poly is not contained.
 * If line between centers intersects with any polygon edge, poly is not contained
 * Only other possibility is that the poly is contained
 */
bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
    // Line line(Geom::center(polygon), Geom::center(*this)); // Centers of polygon
    // for (Line const &edge : polygon.edges()) {
    //     if (Geom::intersects(edge, line))
    //         return false;
    // }

    return true;
}
