#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cstring>
#include "Shape.hpp"
#include <hexdump.hpp>

using std::string;

void framebuffer_size_callback(GLFWwindow* window __attribute__((unused)), int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Minimal OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to open window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint vertexShaderId;
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderId);
    int rc;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &rc);
    if (!rc) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cerr << "ERROR VERTEX COMPILATION FAILED\n" << infoLog << std::endl;
        return -1;
    }

    GLuint fragmentShaderId;
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &rc);
    if (!rc) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
        std::cerr << "ERROR FRAGMENT COMPILATION FAILED\n" << infoLog << std::endl;
        return -1;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderId);
    glAttachShader(shaderProgram, fragmentShaderId);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &rc);
    if (!rc) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR LINKING FAILED\n" << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    // Shape rect({
    //     Point(-0.5, 0.5, 0, Cartesian{}),
    //     Point(-0.5, -0.5, 0, Cartesian{}),
    //     Point(0.5, -0.5, 0, Cartesian{}),
    //     Point(0.5, 0.5, 0, Cartesian{}),
    //     Point(0.5, 0.7, 0, Cartesian{}),
    //     });
    Circle rect(Point(0, 0, 0, Cartesian{}), 0.5, 50);
    GLuint VAOid;
    glGenVertexArrays(1, &VAOid);
    glBindVertexArray(VAOid);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, rect.vtx_size(), rect.vtx_data(), GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint EBOid;
    glGenBuffers(1, &EBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rect.idx_size(), rect.idx_data(), GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
        glDrawElements(GL_TRIANGLES, rect.num_idx(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
