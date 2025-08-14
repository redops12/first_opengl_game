#include <math.h>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shape.hpp"

std::vector<Point> generate_circle(Point center, float radius, unsigned int count) {
    std::vector<Point> vertices;
    for (float t = 0; t < 2*M_PI; t += 2*M_PI/count) {
        vertices.push_back(center + Point(radius, t, 0, Spherical{}));
    }
    return vertices;
}

Circle::Circle(Point center, float radius, unsigned int count): Shape(generate_circle(center, radius, count)) { }
