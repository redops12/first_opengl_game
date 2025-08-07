#include "Shape.hpp"
#include <math.h>

Point::Point(float x, float y, float z, Cartesian): x(x), y(y), z(z) {}
Point::Point(float r, float theta, float phi, Spherical): x(r*sin(theta)*cos(phi)), y(r*sin(theta)*sin(phi)), z(r*cos(theta)) {}

Shape::Shape(const std::vector<Point> &vertices): vertices_(vertices) {
    for (unsigned int i = 1; i < vertices.size() - 1; i++) {
        indices_.push_back(0);
        indices_.push_back(i);
        indices_.push_back(i+1);
    }
}
