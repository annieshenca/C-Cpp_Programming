/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

/*
 * Source:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=2be08498-8350-4514-b8fe-06e265820e39
 * ftp://www.cs.uregina.ca/pub/class/330/ParsingLine/parsingline.html
 */


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

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

static Point3D getPoint3D (std::string &token) {
    double x, y, z;
    std::stringstream vstream(token);
    std::string value;
    if (std::getline(vstream, value, ',')) {
        std::stringstream(value) >> x;
        if (std::getline(vstream, value, ',')) {
            std::stringstream(value) >> y;
            if (std::getline(vstream, value, ',')) {
                std::stringstream(value) >> z;
                return Point3D(x, y, z);
            }
        }
    }
    throw "invalid point " + token;
}

static double getRadius (std::string &token) {
    return std::stod(token);
}

static void parseLine (const std::string line) {
    std::stringstream stream(line);
    std::string token;

    // Containable2D *inner = NULL;
    // Containable2D *outer = NULL;
    Circle *inner = NULL;
    Circle *outer = NULL;
    // Polygon *inner = NULL;
    // Polygon *outer = NULL;
    // Sphere *inner = NULL;
    // Sphere *outer = NULL;
    // Cube *inner = NULL;
    // Cube *outer = NULL;

    bool expected = false;

    while (stream >> token) {
        // If the shape is a Circle.
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
        // If the shape is a Polygon.
        if (token == "Polygon") {
            stream >> token;
            Point2D corner1 = getPoint(token);
            stream >> token;
            Point2D corner2 = getPoint(token);
            stream >> token;
            Point2D corner3 = getPoint(token);
            stream >> token;
            Point2D corner4 = getPoint(token);
            if (inner == NULL)
                inner = new Polygon(corner1, corner2, corner3, corner4);
            else
                outer = new Polygon(corner1, corner2, corner3, corner4);
        }
        // If the shape is a TeuleauxTriangle.
        // if (token == "TeuleauxTriangle") {
            // stream >> token;
            // Point2D center = getPoint(token);
            // if (inner == NULL)
            //     inner = new Circle(center, radius);
            // else
            //     outer = new Circle(center, radius);
        // }
        // If the shape is a Sphere.
        if (token == "Sphere") {
            stream >> token;
            Point3D center = getPoint3D(token);
            stream >> token;
            double radius = getRadius(token);
            if (inner == NULL)
                inner = new Sphere(center, radius);
            else
                outer = new Sphere(center, radius);
        }
        // If the shape is a Cube.
        if (token == "Cube") {
            // Store the upper corner points
            stream >> token;
            Point3D upper1 = getPoint3D(token);
            stream >> token;
            Point3D upper2 = getPoint3D(token);
            stream >> token;
            Point3D upper3 = getPoint3D(token);
            stream >> token;
            Point3D upper4 = getPoint3D(token);
            Point3D upper[4] = {upper1,upper2,upper3,upper4};

            // Store the lower corner points
            stream >> token;
            Point3D lower1 = getPoint3D(token);
            stream >> token;
            Point3D lower2 = getPoint3D(token);
            stream >> token;
            Point3D lower3 = getPoint3D(token);
            stream >> token;
            Point3D lower4 = getPoint3D(token);
            Point3D lower[4] = {lower1,lower2,lower3,lower4};
            if (inner == NULL)
                inner = new Cube(upper, lower);
            else
                outer = new Cube(upper, lower);
        }
        If the shape is a ReuleauxRetrahedron.
        if (token == "ReuleauxRetrahedron") {

        }

        if (token == "true")
            expected = true;
    }
    //bool got = ((Circle *)inner)->containedWithin(*((Circle *)outer));
    bool got = inner->containedWithin(*outer));
    std::cout << (got == expected ? "PASS" : "FAIL" ) << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf(USAGE, argv[0]);
        return -1;
    }
    //std::cout << "Test file: " << argv[1] << std::endl;

    std::ifstream inFile (argv[1]);
    std::string line;
    while (std::getline(inFile, line)) {
        parseLine(line);
    }

    inFile.close();
    return 0;
}
