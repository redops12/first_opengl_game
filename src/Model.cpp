#include <stdexcept>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

#include <glad/glad.h>
#include "Model.hpp"
#include "Shader.hpp"
#include "image_loader.hpp"

using namespace std;

Mesh::Mesh(ShaderProgram &shader, std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, glm::mat4 transform)
    : shader(shader), _vertices(std::move(vertices)), _indices(std::move(indices)), _textures(std::move(textures)), texCount(aiTextureType_UNKNOWN, 0), model(transform) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    cout << endl;
    for(unsigned int i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        Texture &texture = _textures[i];

        if (texture.location == 0) {
            string material_name = "material_" + string(aiTextureTypeToString(texture.type)) + to_string(texCount[texture.type]++);
            texture.location = shader.set(material_name, texture.id);
            cout << material_name << endl;
        } else {
            shader.setLoc(texture.location, i);
        }
        glBindTexture(GL_TEXTURE_2D, _textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(glm::mat4 global) const {
    // bind appropriate textures
    for (unsigned int i = 0; i < _textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, _textures[i].id);
    }
    shader.set("model", global * model);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::loadModel(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Assimp error: " + std::string(importer.GetErrorString()));
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

void Model::processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform) {
    glm::mat4 nodeTransform = aiMatrix4x4ToGlm(node->mTransformation);
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    // Process all meshes in the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, globalTransform));
    }

    // Recursively process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, globalTransform);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        glm::vec2 uv(0.0f, 0.0f);
        if (mesh->mTextureCoords[0]) {
            uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        vertices.emplace_back(position, normal, uv);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process textures
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    for (auto texType = aiTextureType_NONE; texType < aiTextureType_UNKNOWN; texType = static_cast<aiTextureType>(texType + 1)) {
        std::vector<Texture> maps = loadMaterialTextures(material, texType, scene);
        textures.insert(textures.end(), maps.begin(), maps.end());
    }

    return Mesh(shader, std::move(vertices), std::move(indices), std::move(textures), transform);
}

// Suppose you want to return raw RGBA pixel data
struct LoadedTexture {
    int width;
    int height;
    int channels;
    std::vector<unsigned char> data;
};

// Helper
GLuint loadEmbeddedTexture(const aiTexture* tex) {
    LoadedTexture result;

    if (tex->mHeight == 0) {
        // Case 1: Compressed (PNG/JPG data blob)
        // tex->pcData is an array of bytes, mWidth = size in bytes
        size_t sizeInBytes = tex->mWidth;
        const unsigned char* dataPtr = reinterpret_cast<const unsigned char*>(tex->pcData);

        // You now need to feed this memory buffer into your image loader
        // Example using stb_image:
        int w, h, channels;
        unsigned char* pixels = stbi_load_from_memory(dataPtr, (int)sizeInBytes, &w, &h, &channels, 4);
        if (!pixels)
            throw std::runtime_error("Failed to load embedded compressed texture");

        result.width = w;
        result.height = h;
        result.channels = 4;
        result.data.assign(pixels, pixels + (w * h * 4));
        stbi_image_free(pixels);
    } else {
        // Case 2: Raw data (e.g. uncompressed RGBA)
        // mWidth = width, mHeight = height
        result.width = tex->mWidth;
        result.height = tex->mHeight;
        result.channels = 4;
        result.data.resize(tex->mWidth * tex->mHeight * 4);

        // Copy as RGBA
        for (unsigned int y = 0; y < tex->mHeight; ++y) {
            for (unsigned int x = 0; x < tex->mWidth; ++x) {
                const aiTexel& t = tex->pcData[y * tex->mWidth + x];
                size_t idx = (y * tex->mWidth + x) * 4;
                result.data[idx + 0] = t.r;
                result.data[idx + 1] = t.g;
                result.data[idx + 2] = t.b;
                result.data[idx + 3] = t.a;
            }
        }
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            result.width, result.height,
            0, GL_RGBA, GL_UNSIGNED_BYTE,
            result.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    return texID;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const aiScene *scene) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Check if the texture is already loaded
        if (loaded_textures.find(str.C_Str()) != loaded_textures.end()) {
            textures.push_back(loaded_textures[str.C_Str()]);
            continue;
        }

        Texture texture;
        if (str.C_Str()[0] == '*') {
            unsigned int texIndex = atoi(str.C_Str() + 1);
            aiTexture* tex = scene->mTextures[texIndex];
            texture.id = loadEmbeddedTexture(tex);
        } else {
            // Regular file texture
            std::string fileName = directory + "/" + std::string(str.C_Str());
            ImageLoader img(fileName.c_str());
            texture.id = img;
        }

        texture.type = type;
        texture.location = 0; // will be set later
        textures.push_back(texture);
        loaded_textures[string(str.C_Str())] = texture;
    }
    return textures;
}
