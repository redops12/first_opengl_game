#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
private:
    GLuint shaderID;
public:
    Shader(const std::string &filepath);
    inline GLuint getShaderID() const { return shaderID; }
};
