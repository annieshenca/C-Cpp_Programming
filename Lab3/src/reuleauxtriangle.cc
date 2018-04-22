/*
 * Annie Shen (ashen7)
 * CMPS 109 - Harrison
 * Lab 3
 */


#include <iostream>
#include "circle.h"
#include "polygon.h"
#include "geom.h"
#include "reuleauxtriangle.h"

ReuleauxTriangle::ReuleauxTriangle(std::vector<Point2D> vertices) {
    vertices_ = vertices;
    //std:vector<vertices>;
}

std::vector<Point2D> ReuleauxTriangle::vertices() {
    return vertices_;
    //return std:vector<vertices>;
}

std::vector<Line> ReuleauxTriangle::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++)
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}

int ReuleauxTriangle::vertexCount() {
    return vertices_.size();
}

/*
 * Polygon in C
 */
bool ReuleauxTriangle::containedWithin(Circle &circle) {
    // If any of the vertices are outside the circle.
    // for (Point2D const &vertex : vertices()) {
    //     // Imported geom.h to be able to use it here.
    //     if ( Geom::distance(vertex, circle.center()) > circle.radius() )
    //         return false;
    // }
    return true;
}


/*
 * If any poly edge intersects with any polygon edge, poly is not contained.
 * If line between centers intersects with any polygon edge, poly is not contained
 * Only other possibility is that the poly is contained
 */
bool ReuleauxTriangle::containedWithin(RegularConvexPolygon &polygon) {

    // Line line(Geom::center(rt), Geom::center(*this)); // Centers of polygon
    // for (Line const &edge : polygon.edges()) {
    //     if (Geom::intersects(edge, line))
    //         return false;
    // }
    //
    // printf("can this work now please?\n");
    // for (Line const &edge : polygon.edges()) {
    //     for (Line const &inner : edges()) {
    //         if (Geom::intersects(edge, inner)) {
    //             printf("lines intersected!!!\n");
    //             return false;
    //         }
    //     }
    // }

    return true;
}

/*
 * If any poly edge intersects with any polygon edge, poly is not contained.
 * If line between centers intersects with any polygon edge, poly is not contained
 * Only other possibility is that the poly is contained
 */
bool ReuleauxTriangle::containedWithin(ReuleauxTriangle &rt) {

    // Line line(Geom::center(polygon), Geom::center(*this)); // Centers of polygon
    // for (Line const &edge : polygon.edges()) {
    //     if (Geom::intersects(edge, line))
    //         return false;
    // }


    return true;
}
