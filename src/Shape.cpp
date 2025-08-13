#include <math.h>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shape.hpp"

Point::Point(float x, float y, float z, Cartesian): x(x), y(y), z(z) {}
Point::Point(float r, float theta, float phi, Spherical): x(r*sin(theta)*cos(phi)), y(r*cos(theta)), z(r*sin(theta)*sin(phi)) {}
ColorPoint::ColorPoint(float x, float y, float z, float r, float g, float b, Cartesian): Point(x, y, z, Cartesian{}), r(r), g(g), b(b) {}
ColorPoint::ColorPoint(float radius, float theta, float phi, float r, float g, float b, Spherical): Point(radius, theta, phi, Spherical{}), r(r), g(g), b(b) {}
TexturePoint::TexturePoint(float x, float y, float z, float u, float v, Cartesian): Point(x, y, z, Cartesian{}), u(u), v(v) {}
TexturePoint::TexturePoint(float radius, float theta, float phi, float u, float v, Spherical): Point(radius, theta, phi, Spherical{}), u(u), v(v) {}
ColorTexturePoint::ColorTexturePoint(float x, float y, float z, float r, float g, float b, float u, float v, Cartesian) : Point(x, y, z, Cartesian{}), r(r), g(g), b(b), u(u), v(v) {}
ColorTexturePoint::ColorTexturePoint(float radius, float theta, float phi, float r, float g, float b, float u, float v, Spherical) : Point(radius, theta, phi, Spherical{}), r(r), g(g), b(b), u(u), v(v) {}

std::vector<Point> generate_circle(Point center, float radius, unsigned int count) {
    std::vector<Point> vertices;
    for (float t = 0; t < 2*M_PI; t += 2*M_PI/count) {
        vertices.push_back(center + Point(radius, t, 0, Spherical{}));
    }
    return vertices;
}

Circle::Circle(Point center, float radius, unsigned int count): Shape(generate_circle(center, radius, count)) { }
