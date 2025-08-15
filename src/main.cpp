#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <hexdump.hpp>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shape.hpp"
#include "Shader.hpp"
#include "image_loader.hpp"
#include "obj_loader.hpp"
#include "Player.hpp"

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window __attribute__((unused)), int width, int height) {
    glViewport(0, 0, width, height);
}

Player player(glm::vec3(0.0f, 0.0f, -100.0f), 0.0f, 0.0f);
void mouseMoveCallback(__attribute__((unused)) GLFWwindow*  window, double xpos, double ypos) {
    player.mouseCallback(xpos, ypos);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMoveCallback);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ShaderProgram shaderProgram("/home/rwburke/VideoGames/first_run/shaders/basic.vert", "/home/rwburke/VideoGames/first_run/shaders/basic.frag");

    // Shape<ColorTexturePoint> TexRect(
    //         {                 // Vertices           Colors                  Texture
    //         ColorTexturePoint(0.1f,  0.1f,  0.0f,   0.870, 0.490, 0.184,    1.0f, 1.0f, Cartesian{}),
    //         ColorTexturePoint(0.1f,  -0.1f, 0.0f,   0.870, 0.490, 0.184,    1.0f, 0.0f, Cartesian{}),
    //         ColorTexturePoint(-0.1f, -0.1f, 0.0f,   0.513, 0.070, 0.721,    0.0f, 0.0f, Cartesian{}),
    //         ColorTexturePoint(-0.1f, 0.1f,  0.0f,   0.513, 0.070, 0.721,    0.0f, 1.0f, Cartesian{}),
    //         });
    ObjectLoader TexRect("/home/rwburke/VideoGames/first_run/blender/exports/DVDvideo.obj");
    ImageLoader img("/home/rwburke/VideoGames/first_run/resources/dvd-logo-png-19252.png", true);


    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
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

    int uTexLocation = glGetUniformLocation(shaderProgram, "uTex");
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "globColor");
    glUniform1i(uTexLocation, 0);

    float prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float new_time = glfwGetTime();
        float delta = glfwGetTime() - prev_time;
        prev_time = new_time;

        player.processInput(window);
        player.update(delta);

        // create transformations
        mat4 view          = glm::mat4(1.0f);
        mat4 projection    = glm::mat4(1.0f);
        view  = player.getView();
        projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
        // retrieve the matrix uniform locations
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int perspectiveLoc  = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, value_ptr(projection));

        vector<glm::vec3> positions = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(10.0f, 10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, -10.0f),
            glm::vec3(20.0f, 10.0f, 10.0f),
            glm::vec3(-20.0f, -10.0f, -10.0f),
            glm::vec3(20.0f, 10.0f, 10.0f),
            glm::vec3(-20.0f, -10.0f, -10.0f),
            glm::vec3(10.0f, 20.0f, 10.0f),
            glm::vec3(-10.0f, -20.0f, -10.0f),
            glm::vec3(-10.0f, -10.0f, -20.0f),
        };
        for (int i = 0; i < 10; i++) {
            mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = translate(model, positions[i]);
            model = rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(-80.0f), glm::vec3(sin(i+1), cos(3*i+1), sin(i*5+1)));
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
            glDrawElements(GL_TRIANGLES, TexRect.num_idx(), GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
