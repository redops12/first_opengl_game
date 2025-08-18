#include <GLFW/glfw3.h>

class Window {
    private:
        GLFWwindow* window;

    public:
        Window();
        ~Window();
        operator GLFWwindow*() {
            return window;
        }

        float getRatio();
};
