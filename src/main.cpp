#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <hexdump.hpp>
#include <stb/stb_image.h>
#include <glm/glm.hpp>

#include "Shape.hpp"
#include "Shader.hpp"
#include "image_loader.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window __attribute__((unused)), int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    };

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

    Shape<ColorTexturePoint> TexRect(
            {                 // Vertices           Colors                  Texture
            ColorTexturePoint(0.1f,  0.1f,  0.0f,   0.870, 0.490, 0.184,    1.0f, 1.0f, Cartesian{}),
            ColorTexturePoint(0.1f,  -0.1f, 0.0f,   0.870, 0.490, 0.184,    1.0f, 0.0f, Cartesian{}),
            ColorTexturePoint(-0.1f, -0.1f, 0.0f,   0.513, 0.070, 0.721,    0.0f, 0.0f, Cartesian{}),
            ColorTexturePoint(-0.1f, 0.1f,  0.0f,   0.513, 0.070, 0.721,    0.0f, 1.0f, Cartesian{}),
            });
    ImageLoader img("/home/rwburke/VideoGames/first_run/resources/dvd-logo-png-19252.png", true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLuint VAOid;
    glGenVertexArrays(1, &VAOid);
    glBindTexture(GL_TEXTURE_2D, img);
    glBindVertexArray(VAOid);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, TexRect.vtx_size(), TexRect.vtx_data(), GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColorTexturePoint), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColorTexturePoint), (void*)sizeof(Point));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ColorTexturePoint), (void*)sizeof(ColorPoint));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLuint EBOid;
    glGenBuffers(1, &EBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, TexRect.idx_size(), TexRect.idx_data(), GL_STATIC_DRAW);

    glUseProgram(shaderProgram);

    int aLocLocation = glGetUniformLocation(shaderProgram, "aLoc");
    int uTexLocation = glGetUniformLocation(shaderProgram, "uTex");
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "globColor");
    glUniform1i(uTexLocation, 0);

    glm::vec3 velocity(0.55f, 0.35f, 0.0f);
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    float prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float new_time = glfwGetTime();
        float delta = glfwGetTime() - prev_time;
        prev_time = new_time;

        // glUniform4f(vertexColorLocation, (sin(timeValue) / 2.0f) + 0.5f, sin(timeValue + 2*M_PI/3) / 2.0f + 0.5f, sin(timeValue + 4*M_PI/3) / 2.0f + 0.5f, 1.0f);
        // glUniform3f(aLocLocation, cos(timeValue)/2.0f, sin(timeValue) / 2.0f, 0.0f);
        pos += velocity * delta;
        if (pos.x > 0.9) {
            pos.x = 0.9f;
            velocity.x = -velocity.x;
        }
        if (pos.x < -0.9) {
            pos.x = -0.9f;
            velocity.x = -velocity.x;
        }

        if (pos.y > 0.9) {
            pos.y = 0.9f;
            velocity.y = -velocity.y;
        }
        if (pos.y < -0.9) {
            pos.y = -0.9f;
            velocity.y = -velocity.y;
        }
        glUniform3f(aLocLocation, pos.x, pos.y, pos.z);

        glDrawElements(GL_TRIANGLES, TexRect.num_idx(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
