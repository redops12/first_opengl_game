#pragma once

#include <GLFW/glfw3.h>

class ImageLoader {
    private:
        GLuint textureID_;
        void loadImage(const char *filename, bool flipY);

    public:
        ImageLoader(const char *filename, bool flipY = false);
        operator GLuint() const { return textureID_; }
};
