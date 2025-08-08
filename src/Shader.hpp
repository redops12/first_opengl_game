#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
private:
    GLuint shaderID;
public:
    Shader(const std::string &filepath);
    ~Shader();
    // given an implicit conversion to GLuint as a typecast
    operator GLuint() const { return shaderID; }
};

class ShaderProgram {
    private:
        GLuint progID;
    public:
        ShaderProgram(const std::string &vert_filepath, const std::string &frag_filepath);
        operator GLuint() const { return progID; }
};
