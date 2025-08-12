#include <math.h>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shape.hpp"

Point::Point(float x, float y, float z, Cartesian): x(x), y(y), z(z) {}
Point::Point(float r, float theta, float phi, Spherical): x(r*sin(theta)*cos(phi)), y(r*cos(theta)), z(r*sin(theta)*sin(phi)) {}
ColorPoint::ColorPoint(float x, float y, float z, float r, float g, float b, Cartesian): Point(x, y, z, Cartesian{}), r(r), g(g), b(b) {}
ColorPoint::ColorPoint(float radius, float theta, float phi, float r, float g, float b, Spherical): Point(radius, theta, phi, Spherical{}), r(r), g(g), b(b) {}
TexturePoint::TexturePoint(float x, float y, float z, float u, float v, Cartesian): Point(x, y, z, Cartesian{}), u(u), v(v) {}
TexturePoint::TexturePoint(float radius, float theta, float phi, float u, float v, Spherical): Point(radius, theta, phi, Spherical{}), u(u), v(v) {}

std::vector<Point> generate_circle(Point center, float radius, unsigned int count) {
    std::vector<Point> vertices;
    for (float t = 0; t < 2*M_PI; t += 2*M_PI/count) {
        vertices.push_back(center + Point(radius, t, 0, Spherical{}));
    }
    return vertices;
}

Circle::Circle(Point center, float radius, unsigned int count): Shape(generate_circle(center, radius, count)) { }

TexturedShape::TexturedShape(const std::vector<TexturePoint> &vertices, const std::string& pathname, bool flip): Shape<TexturePoint>(vertices) {
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (flip) {
        stbi_set_flip_vertically_on_load(true);
    } else {
        stbi_set_flip_vertically_on_load(false);
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(pathname.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture image: " + pathname);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}
