#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bounds.h"

/*
 * in-lecture sources:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=785d50a3-c653-46b8-b9f6-0c468e959ac7
 */
static Shape *arena;
/*
 * Intialise a two or three dimensional space of shape ARENA containing NUMSHAPES
 * SHAPES, all of which may or may not be entirely contained within ARENA.
 */
void setup(Shape *a, Shape *shapes[], int numShapes) {
    arena = a;
}

/******************************************************************************/
/***************************** Helper Functions *******************************/
/*
 * Find the linear function given two points
 * https://www.geeksforgeeks.org/program-find-line-passing-2-points/
 */
float* lineFromPoints(Point i, Point j) {
    float a = j.y - i.y;
    float b = i.x - j.x;
    float c = a*(i.x) + b*(i.y);
    c = -c;
    float* arr = malloc(sizeof(float)*3);
    arr[0]=a;
    arr[1]=b;
    arr[2]=c;
    return arr;
}
/* Determine if a line and a circle intersect
 * https://www.geeksforgeeks.org/check-line-touches-intersects-circle/
 */
static bool circleLineCollision (int a, int b, int c, Circle *circle) {
    int x = circle->center.x;
    int y = circle->center.y;
    int radius = circle->radius;
    // Find the distance of line from center.
    int dist = (abs(a*x + b*y + c)) / sqrt(pow(a,2) + pow(b,2));
    // Checking if the dist is less than, greater than, or equal to radius.
    if (radius > dist) {
        //printf("intersect\n");
        return false;
    }
    return true;
}

/******************************************************************************/
/******************************************************************************/

/*
 * Check is SHAPE is in ARENA functions
 */
static bool isCircleInsideCircle(Circle *outer, Circle *inner) {
    float distance = sqrt(
        pow ( outer->center.x - inner->center.x, 2 ) +
        pow ( outer->center.y - inner->center.y, 2 ));
    return distance <= outer->radius - inner->radius;
}
static bool isCircleInsidePoly(Polygon *outer, Circle *inner) {
    // For number of pairs of lines in the polygon, check if each line touches
    // the circle.
    int numV = outer->numVertices;
    float* line;
    Point p1;
    Point p2;
    for (int i = 0; i < numV; i++) {
        // First get two points and form a line
        p1 = outer->vertices[i];
        if (i < numV) {
            p2 = outer->vertices[i+1];
        } else { // At the least pair, which the line pairs with the vertices[0]
            p2 = outer->vertices[0];
        }

        // Accheive the linear function and gets back a, b, c in array form
        line = lineFromPoints(p1, p2);

        // Check if line collide with circle
        circleLineCollision(line[0], line[1], line[2], inner);

        // Will get here iff the circle is completely INside or OUTside
        // Which I am unsure of how to solve
        free(line);
    }
    line = NULL;
    return true;
}


/*
 * Move the centroid of SHAPE to POINT.
 *
 * Return TRUE if SHAPE remains entirely within the space defined by the
 * ARENA parameter of setup(), FALSE otherwise.
 * AKA, True until proven False!
 */
bool move(Shape *shape, Point *point) {
    if (arena->type == CIRCLE && shape->type == CIRCLE) {
        Circle *inner = (Circle *) shape;
        // Move center (x,y) of SHAPE by amount in POINT
        inner->center.x = point->x;
        inner->center.y = point->y;
        return isCircleInsideCircle((Circle *)arena, inner);
    }

    if (arena->type == POLYGON && shape->type == CIRCLE) {
        Circle *inner = (Circle *) shape;
        inner->center.x = point->x;
        inner->center.y = point->y;
        return isCircleInsidePoly((Polygon *)arena, inner);
    }

    return true;
}
