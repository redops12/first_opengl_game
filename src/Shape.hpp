#include <vector>

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

class Shape {
    private:
    std::vector<Point> vertices_;
    std::vector<unsigned int> indices_;

    public:
    inline const float* vtx_data() const { return reinterpret_cast<const float *>(vertices_.data()); }
    inline const unsigned int* idx_data() const { return indices_.data(); }
    inline size_t vtx_size() const { return vertices_.size() * sizeof(Point); }
    inline size_t idx_size() const { return indices_.size() * sizeof(unsigned int); }
    inline size_t num_idx() const { return indices_.size(); }

    /**
     * @brief Instantiate a polygon from a list of vertices
     *
     * @param[in] vertices List of vertices in order
     */
    Shape(const std::vector<Point> &vertices);
};

std::vector<Point> generate_circle(Point center, float radius, unsigned int count);
class Circle : public Shape {
    public:
    /**
     * @brief Construct a circle from a center point and a radius
     *
     * @param[in] center as a Point
     * @param[radius] radius As a float between 0 and 1
     */
    Circle(Point center, float radius, unsigned int count = 360);
};
