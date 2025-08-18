#pragma once

#include <string>
#include <unordered_map>
#include <typeinfo>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        std::unordered_map<std::string, GLuint> uniforms;

    public:
        ShaderProgram(const std::string &vert_filepath, const std::string &frag_filepath);
        operator GLuint() const { return progID; }

        template <typename T>
        GLuint set(const std::string &name, const T &value) {
            if (uniforms.find(name) == uniforms.end()) {
                uniforms[name] = glGetUniformLocation(progID, name.c_str());
            }

            GLuint loc = this->uniforms[name];
            setLoc(loc, value);
            return loc;
        }

        template <typename T>
        void setLoc(GLint location, const T& value) {
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
                glUniform1i(location, value);
            } else if constexpr (std::is_same_v<T, float>) {
                glUniform1f(location, value);
            }
            else if constexpr (std::is_same_v<T, glm::vec2>) {
                glUniform2fv(location, 1, glm::value_ptr(value));
            }
            else if constexpr (std::is_same_v<T, glm::vec3>) {
                glUniform3fv(location, 1, glm::value_ptr(value));
            }
            else if constexpr (std::is_same_v<T, glm::vec4>) {
                glUniform4fv(location, 1, glm::value_ptr(value));
            }
            else if constexpr (std::is_same_v<T, glm::mat4>) {
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
            }
            else {
                static_assert([]{return false;}(), "Unsupported uniform type");
            }
        }
};
