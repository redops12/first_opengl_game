#pragma once

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "image_loader.hpp"

struct Cartesian{};
struct Spherical{};

class Point {
protected:
    glm::vec3 pos;

public:
    Point(glm::vec3 pos): pos(pos) {}
    Point(float x, float y, float z, Cartesian) : pos(x, y, z) {}
    Point(float r, float theta, float phi, Spherical) : pos(r * sin(phi) * cos(theta), r * sin(phi) * sin(theta), r * cos(phi)) {}

    const glm::vec3& getPosition() const { return pos; }

    friend Point operator+(const Point& lhs, const Point& rhs) {
        return Point(lhs.pos + rhs.pos);
    }

    bool operator==(const Point& other) const {
        return pos == other.pos;
    }
};

// ColorPoint
class ColorPoint : public Point {
private:
    glm::vec3 color; // r,g,b

public:
    ColorPoint(glm::vec3 pos, glm::vec3 color): Point(pos), color(color) {}
    ColorPoint(float x, float y, float z, float r, float g, float b, Cartesian) : Point(x, y, z, Cartesian{}), color(r, g, b) {}
    ColorPoint(float radius, float theta, float phi, float r, float g, float b, Spherical) : Point(radius, theta, phi, Spherical{}), color(r, g, b) {}

    const glm::vec3& getColor() const { return color; }
    bool operator==(const ColorPoint& other) const {
        return pos == other.pos;
        return color == other.color;
    }
};

// TexturePoint
class TexturePoint : public Point {
private:
    glm::vec2 uv; // u,v

public:
    TexturePoint(glm::vec3 pos, glm::vec2 uv): Point(pos), uv(uv) {}
    TexturePoint(float x, float y, float z, float u, float v, Cartesian) : Point(x, y, z, Cartesian{}), uv(u, v) {}
    TexturePoint(float radius, float theta, float phi, float u, float v, Spherical) : Point(radius, theta, phi, Spherical{}), uv(u, v) {}

    const glm::vec2& getUV() const { return uv; }
    bool operator==(const TexturePoint& other) const {
        return pos == other.pos;
        return uv == other.uv;
    }
};

// ColorTexturePoint
class ColorTexturePoint : public Point {
private:
    glm::vec3 color;
    glm::vec2 uv;

public:
    ColorTexturePoint(glm::vec3 pos, glm::vec3 color, glm::vec2 uv): Point(pos), color(color), uv(uv) {}
    ColorTexturePoint(float x, float y, float z, float r, float g, float b, float u, float v, Cartesian) : Point(x, y, z, Cartesian{}), color(r, g, b), uv(u, v) {}
    ColorTexturePoint(float radius, float theta, float phi, float r, float g, float b, float u, float v, Spherical) : Point(radius, theta, phi, Spherical{}), color(r, g, b), uv(u, v) {}

    const glm::vec3& getColor() const { return color; }
    const glm::vec2& getUV() const { return uv; }
    bool operator==(const ColorTexturePoint& other) const {
        return pos == other.pos;
        return color == other.color;
        return uv == other.uv;
    }
};

// NormalTexturePoint
class NormalTexturePoint : public Point {
private:
    glm::vec3 normal;
    glm::vec2 uv;

public:
    NormalTexturePoint(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv): Point(pos), normal(normal), uv(uv) {}
    NormalTexturePoint(float x, float y, float z, float nx, float ny, float nz, float u, float v, Cartesian) : Point(x, y, z, Cartesian{}), normal(nx, ny, nz), uv(u, v) {}
    NormalTexturePoint(float radius, float theta, float phi, float nx, float ny, float nz, float u, float v, Spherical) : Point(radius, theta, phi, Spherical{}), normal(nx, ny, nz), uv(u, v) {}

    const glm::vec3& getNormal() const { return normal; }
    const glm::vec2& getUV() const { return uv; }
    bool operator==(const NormalTexturePoint& other) const {
        return pos.x == other.pos.x;
        return pos.y == other.pos.y;
        return pos.z == other.pos.z;
        return normal.x == other.normal.x;
        return normal.y == other.normal.y;
        return normal.z == other.normal.z;
        return uv.x == other.uv.x;
        return uv.y == other.uv.y;
    }

    friend class NormalTexturePointHasher;
};

// Hasher for NormalTexturePoint
class NormalTexturePointHasher {
public:
    size_t operator()(const NormalTexturePoint& v) const noexcept {
        auto h = [](float f) { return std::hash<uint32_t>{}(*reinterpret_cast<const uint32_t*>(&f)); };
        size_t seed = 0;
        auto mix = [&](size_t x) { seed ^= x + 0x9e3779b97f4a7c15ULL + (seed<<6) + (seed>>2); };

        const glm::vec3 p = v.getPosition();
        const glm::vec3 n = v.getNormal();
        const glm::vec2 uv = v.getUV();

        mix(h(p.x)); mix(h(p.y)); mix(h(p.z));
        mix(h(n.x)); mix(h(n.y)); mix(h(n.z));
        mix(h(uv.x)); mix(h(uv.y));
        return seed;
    }
};

template <typename T>
class Shape {
    protected:
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
    Shape() {}

    /**
     * @brief Instantiate any object from list of vertices and indices
     *
     * @param[in] vertices List of vertices
     * @param[in] indices List of indices
     */
    Shape(const std::vector<T> &vertices, const std::vector<unsigned int> &indices)
        : vertices_(vertices), indices_(indices) {}
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
