#include <vector>

struct Cartesian{};
struct Spherical{};

class Point {
    private:
    float x, y, z;

    public:
    Point(float x, float y, float z, Cartesian);
    Point(float r, float theta, float phi, Spherical);
};

class Shape {
    private:
    std::vector<Point> vertices_;
    std::vector<unsigned int> indices_;

    public:
    inline const std::vector<Point>& getVertices() const { return vertices_; }
    inline const std::vector<unsigned int>& getIndices() const { return indices_; }

    /**
     * @brief Instantiate a polygon from a list of vertices
     *
     * @param[in] vertices List of vertices in order
     */
    Shape(const std::vector<Point> &vertices);
};

class Circle : Shape {
    /**
     * @brief Construct a circle from a center point and a radius
     *
     * @param[in] center as a Point
     * @param[radius] radius As a float between 0 and 1
     */
    Circle(Point center, float radius);
};
