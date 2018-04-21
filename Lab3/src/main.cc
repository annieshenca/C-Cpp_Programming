/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

/*
 * Resources:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=956436f9-b748-4ab7-b7b7-2e27605f1667
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=7a0e717d-dfce-448e-b40b-c6c7cd057755
*/


#include <iostream>
#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"
using namespace std;


/* Pass in msg, expected result, and what the function actually return.
 * Returns PASS or FAIL for each test. */
static void results (const char *msg, bool expected, bool got)
{
    cout << (expected == got ? "PASS " : "FAIL ") << msg << endl;
}

/*
 *
 */
// static void printVertices (RegularConvexPolygon poly) {
//     for (Point2D const &vertex : poly.vertices()) {
//         printf("(%3.lf,%3.lf) ", vertex.x, vertex.y);
//     }
//     cout << endl;
// }

// static void printCenter (Circle &circle) {
//     Point2D const &center : circle.center();
//     //printf("(%3.lf)", circle.center());
//     printf("(%3.lf)", center);
//     std::cout << std::endl;
// }

/**************************************************************************
 * Test cases for CIRCLE OUTER.
 */
static void testCirclesInsideCircleT ()
{
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            Circle inner = Circle(Point2D(i*2.0,j*2.0), 4.0);
            //printCenter(inner);
            results("Circles-inside-CircleT", true, inner.containedWithin(outer));
        }
    }
}

static void testCirclesInsideCircleF ()
{
    Circle outer = Circle(Point2D(0.0,0.0), 3.0);

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            Circle inner = Circle(Point2D(i*2.0,j*2.0), 2.5);
            //printCenter(inner);
            results("Circles-inside-CircleF", false, inner.containedWithin(outer));
        }
    }
}

//-----------------------//
static void testTrianglesInsideCircle () {
    Circle outer = Circle(Point2D(0.0,0.0), 5.0);

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            RegularConvexPolygon inner =
                RegularConvexPolygon({ Point2D(i*-1,j*-1),
                    Point2D( i*0, j*2), Point2D( i*1,j*-1)});
            //printCenter(inner);
            //printVertices(inner);
            results("Triangle-in-Circle", true, inner.containedWithin(outer));
        }
    }
}

static void testSquaresInsideCircle () {
    Circle outer = Circle(Point2D(0.0,0.0), 5.0);

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            RegularConvexPolygon inner =
                RegularConvexPolygon({
                    Point2D( i*2, j*1), Point2D( i*2,j*-1),
                    Point2D(i*-2, j*1), Point2D(i*-2,j*-1) });
            //printCenter(inner);
            //printVertices(inner);
            results("Squares-in-Circle", true, inner.containedWithin(outer));
        }
    }
}

static void testHexagonsInsideCircle () {
    Circle outer = Circle(Point2D(0.0,0.0), 12.0);

    RegularConvexPolygon inner =
        RegularConvexPolygon({
            Point2D( i*2, j*1), Point2D( i*2,j*-1),
            Point2D(i*-2, j*1), Point2D(i*-2,j*-1) });
    //printCenter(inner);
    //printVertices(inner);
    results("Squares-in-Circle", true, inner.containedWithin(outer));
}
//-----------------------//

/**************************************************************************
 * Test cases for POLYGON OUTER.
 */
// static void testSquaresInsideSquare()
// {
//     RegularConvexPolygon outer =
//         RegularConvexPolygon({
//             Point2D( 10, 10), Point2D( 10,-10),
//             Point2D(-10, 10), Point2D(-10,-10) });
//
//     for (int i = -1; i <= 1; i += 2) {
//         for (int j = -1; j <= 1; j += 2) {
//             RegularConvexPolygon inner =
//                 RegularConvexPolygon({
//                     Point2D( i*3, j*6), Point2D( i*3,j*-6),
//                     Point2D(i*-3, j*6), Point2D(i*-3,j*-6) });
//
//             //printVertices(inner);
//             results("Squares-inside-Square", true, inner.containedWithin(outer));
//         }
//     }
// }






/**************************************************************************
 * Test cases for RTRIANGLE OUTER.
 */
// static void testTrianglesInRTriangle()
// {
//
//     RegularConvexPolygon inner =
//         RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});
//
//     results("Triangle-in-Circle", true, inner.containedWithin(outer));
// }



/**************************************************************************
 * Main function. Contains all tests for shape functions.
 */
int main(int argc, char *argv[])
{
    testCirclesInsideCircleT();
    testCirclesInsideCircleF();
    testTrianglesInsideCircle();
    testSquaresInsideCircle();
    testHexagonsInsideCircle();
    printf("------\n");
    //testSquaresInsideSquare();

    printf("------\n");


    printf("------\n");
}





//
