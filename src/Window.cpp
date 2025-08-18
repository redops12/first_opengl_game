#include <stdexcept>

#include <glad/glad.h>

#include "Window.hpp"


float glob_width = 800;
float glob_height = 600;
void framebuffer_size_callback(GLFWwindow* window __attribute__((unused)), int width, int height) {
    glob_width = width;
    glob_width = height;
    glViewport(0, 0, width, height);
}

Window::Window() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    };

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(glob_width, glob_height, "Minimal OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to open window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, glob_width, glob_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

float Window::getRatio() {
    return glob_width / glob_height;
}
