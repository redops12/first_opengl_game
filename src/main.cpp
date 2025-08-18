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

#include "Shader.hpp"
#include "image_loader.hpp"
#include "Player.hpp"
#include "Model.hpp"
#include "Window.hpp"
#include "Color.hpp"

using namespace std;
using namespace glm;

Player player(glm::vec3(0.0f, 0.0f, -10.0f), 0.0f, 0.0f);
void mouseMoveCallback(__attribute__((unused)) GLFWwindow*  window, double xpos, double ypos) {
    player.mouseCallback(xpos, ypos);
}

int main() {
    Window window;

    ShaderProgram shaderProgram("/home/rwburke/VideoGames/first_run/shaders/basic.vert", "/home/rwburke/VideoGames/first_run/shaders/basic.frag");
    Model bookshelf(shaderProgram, "/home/rwburke/VideoGames/shared_resources/blender/exports/cube_glb/cube.glb", 0.3);
    Model katana(shaderProgram, "/home/rwburke/VideoGames/shared_resources/resources/dragon_katana/source/dragon_katana_oni_koroshi.glb", 1.0);
    Model light(shaderProgram, "/home/rwburke/VideoGames/shared_resources/resources/light/sci-fi_tripod_light.glb", 0.8);
    Model room(shaderProgram, "/home/rwburke/VideoGames/shared_resources/blender/exports/room/room.glb", 0.0);
    Model table(shaderProgram, "/home/rwburke/VideoGames/shared_resources/resources/pool_table/pool_table.glb", 0.0);

    glfwSetCursorPosCallback(window, mouseMoveCallback);

    glUseProgram(shaderProgram);

    shaderProgram.set("Color", vec3(0.5f, 0.5f, 0.5f));

    shaderProgram.set("uTex", 0);

    float prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float new_time = glfwGetTime();
        float delta = glfwGetTime() - prev_time;
        prev_time = new_time;

        player.processInput(window);
        player.update(delta);

        shaderProgram.set("view", player.getView());
        shaderProgram.set("projection", player.getProjection(window.getRatio()));
        shaderProgram.set("cameraPos", player.getPosition());

        mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        bookshelf.Draw(model);

        model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = translate(model, glm::vec3(-2.5f, 1.5f, 0.0f));
        model = rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        katana.Draw(model);

        room.Draw(glm::mat4(1.0f));
        table.Draw(glm::mat4(1.0f));

        vec3 lightPos = glm::vec3(0.0, 2.0, 0.0);
        model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = translate(model, lightPos);
        light.Draw(model);
        shaderProgram.set("light.position", lightPos + vec3(0.0, 1.5, 0.0));
        shaderProgram.set("light.ambient", 1.3f * ambient_colors[CANDLE]);
        shaderProgram.set("light.diffuse", 10.0f * ambient_colors[CANDLE]);
        shaderProgram.set("light.specular", 0.1f * ambient_colors[CANDLE]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
