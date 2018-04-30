/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

/*
 * Source:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=2be08498-8350-4514-b8fe-06e265820e39
 */


#include <iostream>
#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"
#include "sphere.h"
#include "cube.h"
#include "reuleauxtetrahedron.h"

#define USAGE "USAGE %s test-file\n"


static Point2D getPoint (std::string &token) {
    double x, y;
    std::stringstream vstream(token);
    std::string value;
    if (std::getline(vstream, value, ',')) {
        std::stringstream(value) >> x;
        if (std::getline(vstream, value, ',')) {
            std::stringstream(value) >> y;
            return Point2D(x, y);
        }
    }
    throw "invalid point " + token;
}

static double getRadius (std::string &token) {
    return std::stod(token);
}

static void parse (const std::string line) {
    std::stringstream(line);
    std::string token;

    Circle *inner = NULL;
    Circle *outer = NULL;
    bool expected = false;

    while (stream >> token) {
        if (token == "Circle") {
            stream >> token;
            Point2D center = getPoint(token);
            stream >> token;
            double radius = getRadius(token);
            if (inner == NULL)
                inner = new Circle(center, radius);
            else
                outer = new Circle(center, radius);
        }
        if (token == "true")
            expected = true;
    }
    bool got = inner->containedWithin(*outer);
    std::cout << (got == expected ? "PASS" : "FAIL" ) << std::endl;
}

int main(int argc, char *argv[])
{

    if (argc < 2) {
        printf(USAGE, argv[0]);
        return -1;
    }
    std::cout << "Test file: " << argv[1] << std::endl;

    // std::string line;
    // while (std::getline(argv[1], line))
    // {
    //     parse(line);
    // }

    std::ifstream input (argv[1]);
    for (std::string line; std::getline(input, line)) {
        parse(line);
    }

    return 0;
}
