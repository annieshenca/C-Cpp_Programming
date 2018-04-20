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

/*
 *
 */
static void testCirclesInCircle()
{
    Circle inner = Circle(Point2D(0.0,0.0), 4.0);
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);

    results("Circle-in-Circle: ", true, inner.containedWithin(outer));
}

/*
 *
 */
static void testTrianglesInCircle()
{
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    RegularConvexPolygon inner =
        RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});

    results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

/*
 *
 */
static void testTrianglesInCircle()
{
    Circle outer = Circle(Point2D(0.0,0.0), 8.0);
    RegularConvexPolygon inner =
        RegularConvexPolygon({ Point2D(-1,-1), Point2D(0,2), Point2D(1,-1)});

    results("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}


















/*
 *
 */
int main(int argc, char *argv[])
{
    testCirclesInCircle();
    testTrianglesInCircle();
}














//
