#include <string>
#include <fstream>
#include <sstream>
#include <exception>

#include "Shader.hpp"

std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Failed to open shader file: " + filepath);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Load a shader from the passed in path and compile it with glfw
 *
 * @param[in] name The full path of the file to compile
 * .frag for fragment shader
 * .vert for vertex shader
 */
Shader::Shader(const std::string &filepath) {
    // Load shader source code
    std::string source = loadShaderSource(filepath);

    // condition to determine shader type
    unsigned int shaderType = 0;
    if (filepath.find(".vert") != std::string::npos) {
        // Vertex shader
        shaderType = GL_VERTEX_SHADER;
    } else if (filepath.find(".frag") != std::string::npos) {
        // Fragment shader
        shaderType = GL_FRAGMENT_SHADER;
    } else {
        throw std::runtime_error("Unsupported shader type in file: " + filepath);
    }

    // Create shader object
    this->shaderID = glCreateShader(shaderType);

    // Compile the shader
    const char* src = source.c_str();
    glShaderSource(this->shaderID, 1, &src, nullptr);
    glCompileShader(this->shaderID);

    // Check for compilation errors
    int success;
    glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        throw std::runtime_error("Shader compilation failed for " + filepath + ": " + std::string(infoLog));
    }
}

Shader::~Shader() {
    if (this->shaderID != 0) {
        glDeleteShader(this->shaderID);
        this->shaderID = 0;
    }
}

ShaderProgram::ShaderProgram(const std::string &vert_filepath, const std::string &frag_filepath) {
    // Create shader objects
    Shader vertexShader(vert_filepath);
    Shader fragmentShader(frag_filepath);

    // Create shader program
    this->progID = glCreateProgram();

    // Attach shaders to the program
    glAttachShader(this->progID, vertexShader);
    glAttachShader(this->progID, fragmentShader);

    // Link the program
    glLinkProgram(this->progID);

    // Check for linking errors
    int success;
    glGetProgramiv(this->progID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->progID, 512, nullptr, infoLog);
        throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
    }
}
