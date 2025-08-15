#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Player {
    private:
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 look;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        double pitch, yaw;
        double prevX, prevY;
        double mouseSensitivity = 0.1f;

        void recalcLook();
    public:
        Player(glm::vec3 pos, double pitch, double yaw);

        void processInput(GLFWwindow* window);
        void mouseCallback(double xpos, double ypos);
        void update(float delta);
        inline glm::mat4 getView() {
            return glm::lookAt(position, look + position, up);
        }
};
