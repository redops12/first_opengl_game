#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <hexdump.hpp>

#include "Shape.hpp"
#include "Shader.hpp"

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

    ShaderProgram shaderProgram("/home/rwburke/VideoGames/first_run/shaders/basic.vert", "/home/rwburke/VideoGames/first_run/shaders/basic.frag");

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

        float timeValue = glfwGetTime();
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "globColor");
        glUniform4f(vertexColorLocation, (sin(timeValue) / 2.0f) + 0.5f, sin(timeValue + 2*M_PI/3) / 2.0f + 0.5f, sin(timeValue + 4*M_PI/3) / 2.0f + 0.5f, 1.0f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
        glDrawElements(GL_TRIANGLES, rect.num_idx(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
