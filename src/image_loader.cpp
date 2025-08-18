#include "image_loader.hpp"

#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ImageLoader::ImageLoader(const char *filename, bool flipY) {
    int width, height, channels;

    if (flipY) {
        // Flip the image vertically
        stbi_set_flip_vertically_on_load(true);
    } else {
        // Do not flip the image
        stbi_set_flip_vertically_on_load(false);
    }

    if (!filename) {
        throw std::invalid_argument("Filename cannot be null");
    }

    // Force RGBA so every pixel is 4 bytes
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        throw std::runtime_error("Failed to load texture image: " + std::string(filename));
    }

    glGenTextures(1, &this->textureID_);
    glBindTexture(GL_TEXTURE_2D, this->textureID_);

    // Make sure OpenGL reads tightly-packed rows
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Upload image to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Free CPU copy of the image
    stbi_image_free(data);
}
