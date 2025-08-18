#include <vector>
#include <glm/glm.hpp>

enum ambient_colors {
    CANDLE,
    TUNGSTEN_40W,
    TUNGSTEN_100W,
    HALOGEN,
    CARBON_ARC,
    HIGH_NOON_SUN,
    DIRECT_SUNLIGHT,
    OVERCAST_SKY,
    CLEAR_BLUE_SKY,
    WARM_FLUORESCENT,
    STANDARD_FLUORESCENT,
    COOL_WHITE_FLUORESCENT,
    FULL_SPECTRUM_FLUORESCENT,
    GROW_LIGHT_FLUORESCENT,
    BLACK_LIGHT_FLUORESCENT,
    MERCURY_VAPOR,
    SODIUM_VAPOR,
    METAL_HALIDE,
    HIGH_PRESSURE_SODIUM,
};

static std::vector<glm::vec3> ambient_colors = {
    glm::vec3(255.0f / 255.0f, 147.0f / 255.0f, 41.0f / 255.0f),  // CANDLE
    glm::vec3(255.0f / 255.0f, 197.0f / 255.0f, 143.0f / 255.0f), // TUNGSTEN_40W
    glm::vec3(255.0f / 255.0f, 214.0f / 255.0f, 170.0f / 255.0f), // TUNGSTEN_100W
    glm::vec3(255.0f / 255.0f, 241.0f / 255.0f, 224.0f / 255.0f), // HALOGEN
    glm::vec3(255.0f / 255.0f, 250.0f / 255.0f, 244.0f / 255.0f), // CARBON_ARC
    glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 251.0f / 255.0f), // HIGH_NOON_SUN
    glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f), // DIRECT_SUNLIGHT
    glm::vec3(201.0f / 255.0f, 226.0f / 255.0f, 255.0f / 255.0f), // OVERCAST_SKY
    glm::vec3(64.0f / 255.0f, 156.0f / 255.0f, 255.0f / 255.0f),  // CLEAR_BLUE_SKY
    glm::vec3(255.0f / 255.0f, 244.0f / 255.0f, 229.0f / 255.0f), // WARM_FLUORESCENT
    glm::vec3(244.0f / 255.0f, 255.0f / 255.0f, 250.0f / 255.0f), // STANDARD_FLUORESCENT
    glm::vec3(212.0f / 255.0f, 235.0f / 255.0f, 255.0f / 255.0f), // COOL_WHITE_FLUORESCENT
    glm::vec3(255.0f / 255.0f, 244.0f / 255.0f, 242.0f / 255.0f), // FULL_SPECTRUM_FLUORESCENT
    glm::vec3(255.0f / 255.0f, 239.0f / 255.0f, 247.0f / 255.0f), // GROW_LIGHT_FLUORESCENT
    glm::vec3(167.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f),   // BLACK_LIGHT_FLUORESCENT
    glm::vec3(216.0f / 255.0f, 247.0f / 255.0f, 255.0f / 255.0f), // MERCURY_VAPOR
    glm::vec3(255.0f / 255.0f, 209.0f / 255.0f, 178.0f / 255.0f), // SODIUM_VAPOR
    glm::vec3(242.0f / 255.0f, 252.0f / 255.0f, 255.0f / 255.0f), // METAL_HALIDE
    glm::vec3(255.0f / 255.0f, 183.0f / 255.0f, 76.0f / 255.0f),  // HIGH_PRESSURE_SODIUM
};
