#pragma once

#include <GLFW/glfw3.h>

class ImageLoader {
    private:
        GLuint textureID_;

    public:
        ImageLoader(const char *filename, bool flipY = false);
        operator GLuint() const { return textureID_; }
};
