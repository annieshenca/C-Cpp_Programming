/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include "circle.h"
#include "polygon.h"
#include "geom.h"

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

bool RegularConvexPolygon::containedWithin(Circle &circle) {
    // If any of the vertices are outside the circle.
    for (Point2D const &vertex : vertices()) {
        // Imported geom.h to be able to use it here.
        if ( Geom::distance(vertex, circle.center()) > circle.radius() )
            return false;

    }
    return true;
}

bool RegularConvexPolygon::containedWithin(RegularConvexPolygon &polygon) {
    // If intersecting sides, return false.
    for (Line const &edge: polygon.edges()) {
        for (Line const &corner: this) {
            if (Geom::intersects(edge, corner))
                return false;
        }
    }

    // If any vertex is outside the range's vertices, return false.
    // for (Line const &edge: polygon.edges()) {
    //     if (edge.x > *this.x || edge.y > *this.y) {
    //         return false;
    //     }
    // }

    // Else, return true.
    return true;
}

bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
    throw "Not implemented";
}
