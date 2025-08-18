#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include "Shader.hpp"

class Vertex {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv)
            : position(pos), normal(norm), uv(uv) {}
        bool operator==(const Vertex &other) const {
            return position == other.position && normal == other.normal && uv == other.uv;
        }
};

class Texture {
    public:
        GLuint id;
        aiTextureType type;
        GLuint location;
};

class Mesh {
    private:
        ShaderProgram &shader;
        std::vector<Vertex> _vertices;
        std::vector<GLuint> _indices;
        std::vector<Texture> _textures;
        GLuint VAO, VBO, EBO;
        std::vector<unsigned int> texCount;
        glm::mat4 model = glm::mat4(1.0f);

    public:
    Mesh(ShaderProgram &shader,
            std::vector<Vertex> vertices, std::vector<GLuint> indices,
            std::vector<Texture> textures,
            glm::mat4 transform
            );

    void Draw(glm::mat4 model) const;
};

class Model {
    public:
        Model(ShaderProgram &shader, const std::string &path, float roughness): shader(shader), roughness(roughness) {
            loadModel(path);
        }

        void Draw(glm::mat4 model) const {
            shader.set("roughness", roughness);
            for (const Mesh &mesh : meshes) {
                mesh.Draw(model);
            }
        }

    private:
        ShaderProgram &shader;
        std::vector<Mesh> meshes;
        std::string directory;
        std::unordered_map<std::string, Texture> loaded_textures;
        float roughness;

        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const aiScene *scene);
};
