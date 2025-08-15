#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Player.hpp"

void Player::recalcLook() {
    this->look = glm::vec3(
            cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw)),
            sin(glm::radians(this->pitch)),
            cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw))
            );
    this->look = glm::normalize(this->look);
}

Player::Player(glm::vec3 pos, double pitch, double yaw):
    position(pos), velocity(glm::vec3(0.0f, 0.0f, 0.0f)), pitch(pitch), yaw(yaw) {
        recalcLook();
    }

void Player::update(float delta) {
    position += velocity * delta;
    velocity = velocity * 0.9f;
}

glm::mat3 projectxz = {
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

glm::mat3 rot90 = {
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
};

glm::mat3 rot180 = {
    -1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -1.0f,
};

glm::mat3 rot270 = {
    0.0f, 0.0f, -1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
};

void Player::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Add more input handling as needed
    // wasd for movement
    glm::vec3 new_velocity = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        new_velocity += projectxz * look;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        new_velocity += rot270 * projectxz * look;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        new_velocity += rot180 * projectxz * look;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        new_velocity += rot90 * projectxz * look;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        new_velocity += glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        new_velocity += glm::vec3(0.0f, -1.0f, 0.0f);
    }
    this->velocity = (new_velocity == glm::vec3(0.0f)) ? velocity : 30.0f * glm::normalize(new_velocity);
}

void Player::mouseCallback(double xpos, double ypos) {
    double xoff = xpos - this->prevX;
    double yoff = ypos - this->prevY;
    this->prevX = xpos;
    this->prevY = ypos;

    this->pitch -= yoff * mouseSensitivity;
    this->yaw -= xoff * mouseSensitivity;

    if(this->pitch > 89.0f)
        this->pitch =  89.0f;
    if(this->pitch < -89.0f)
        this->pitch = -89.0f;
    recalcLook();
}
