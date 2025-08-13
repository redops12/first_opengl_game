#pragma once

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include "image_loader.hpp"

struct Cartesian{};
struct Spherical{};

class Point {
    private:
    float x, y, z;

    public:
    Point(float x, float y, float z, Cartesian);
    Point(float r, float theta, float phi, Spherical);
    inline friend Point operator+(const Point &lhs, const Point &rhs) {
        return Point(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, Cartesian{});
    }
};

class ColorPoint : public Point {
    private:
    float r, g, b;

    public:
    ColorPoint(float x, float y, float z, float r, float g, float b, Cartesian);
    ColorPoint(float radius, float theta, float phi, float r, float g, float b, Spherical);
};

class TexturePoint : public Point {
    private:
    float u, v;

    public:
    TexturePoint(float x, float y, float z, float u, float v, Cartesian);
    TexturePoint(float radius, float theta, float phi, float u, float v, Spherical);
};

class ColorTexturePoint : public Point {
    private:
    float r, g, b, u, v;

    public:
    ColorTexturePoint(float x, float y, float z, float r, float g, float b, float u, float v, Cartesian);
    ColorTexturePoint(float radius, float theta, float phi, float r, float g, float b, float u, float v, Spherical);
};

template <typename T>
class Shape {
    private:
    std::vector<T> vertices_;
    std::vector<unsigned int> indices_;

    public:
    inline const float* vtx_data() const { return reinterpret_cast<const float *>(vertices_.data()); }
    inline const unsigned int* idx_data() const { return indices_.data(); }
    inline size_t vtx_size() const { return vertices_.size() * sizeof(T); }
    inline size_t idx_size() const { return indices_.size() * sizeof(unsigned int); }
    inline size_t num_idx() const { return indices_.size(); }

    /**
     * @brief Instantiate a polygon from a list of vertices
     *
     * @param[in] vertices List of vertices in order
     */
    Shape(const std::vector<T> &vertices): vertices_(vertices) {
        for (unsigned int i = 1; i < vertices.size() - 1; i++) {
            indices_.push_back(0);
            indices_.push_back(i);
            indices_.push_back(i+1);
        }
    }
};

std::vector<Point> generate_circle(Point center, float radius, unsigned int count);
class Circle : public Shape<Point> {
    public:
    /**
     * @brief Construct a circle from a center point and a radius
     *
     * @param[in] center as a Point
     * @param[radius] radius As a float between 0 and 1
     */
    Circle(Point center, float radius, unsigned int count = 360);
};
