/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <iostream>

#include "circle.h"
#include "polygon.h"

/* Pass in msg, expected result, and what the function actually return.
 * Returns PASS or FAIL for each test.
 */
static void results (const char *msg, bool expected, bool got)
{
    std::cout << msg;
    if (expected == got)
        std::cout << "PASS\n";
    else
        std::cout << "FAIL\n";
}

/**************************************************************************
 * Test cases for CIRCLE OUTER.
 */
static void testCirclesInCircle1() {
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    Circle inner = Circle(Point2D(0.0,0.0), 4.0);

    results("Circle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testCirclesInCircle2() {
    Circle outer = Circle(Point2D(0.0,-1.0), 8.0);
    Circle inner = Circle(Point2D(2.0,2.0), 4.0);

    results("Circle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testCirclesInCircle3() {
    Circle outer = Circle(Point2D(1.0,-3.33), 8.0);
    Circle inner = Circle(Point2D(6.0,2.021), 4.0);

    results("Circle-in-Circle: ", false, inner.containedWithin(outer));
}

static void testCirclesInCircle4() {
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    Circle inner = Circle(Point2D(12.0,0.0), 5.0); // Touch the side outside

    results("Circle-in-Circle: ", false, inner.containedWithin(outer));
}
//==========================================//

static void testTrianglesInCircle() {
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    RegularConvexPolygon inner =
        RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});

    results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testTrianglesInCircle() {
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    RegularConvexPolygon inner =
        RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});

    results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testTrianglesInCircle() {
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    RegularConvexPolygon inner =
        RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});

    results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testTrianglesInCircle() {
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    RegularConvexPolygon inner =
        RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});

    results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

/**************************************************************************
 * Test cases for POLYGON OUTER.
 */




/**************************************************************************
 * Test cases for RTRIANGLE OUTER.
 */
// static void testTrianglesInRTriangle()
// {
//
//     RegularConvexPolygon inner =
//         RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});
//
//     results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
// }



/**************************************************************************
 * Main function. Contains all tests for shape functions.
 */
int main(int argc, char *argv[])
{
    testCirclesInCircle1();
    testCirclesInCircle2();
    testCirclesInCircle3();
    testCirclesInCircle4();
    testTrianglesInCircle();
}














//
