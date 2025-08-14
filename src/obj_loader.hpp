#pragma once

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shape.hpp"

class ObjectLoader : public Shape<NormalTexturePoint> {
    public:
        ObjectLoader(const std::string& path);
};
