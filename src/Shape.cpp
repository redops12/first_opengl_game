#include "Shape.hpp"
#include <math.h>

Point::Point(float x, float y, float z, Cartesian): x(x), y(y), z(z) {}
Point::Point(float r, float theta, float phi, Spherical): x(r*sin(theta)*cos(phi)), y(r*cos(theta)), z(r*sin(theta)*sin(phi)) {}

Shape::Shape(const std::vector<Point> &vertices): vertices_(vertices) {
    for (unsigned int i = 1; i < vertices.size() - 1; i++) {
        indices_.push_back(0);
        indices_.push_back(i);
        indices_.push_back(i+1);
    }
}

std::vector<Point> generate_circle(Point center, float radius, unsigned int count) {
    std::vector<Point> vertices;
    for (float t = 0; t < 2*M_PI; t += 2*M_PI/count) {
        vertices.push_back(center + Point(radius, t, 0, Spherical{}));
    }
    return vertices;
}

Circle::Circle(Point center, float radius, unsigned int count): Shape(generate_circle(center, radius, count)) { }
