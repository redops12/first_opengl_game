#include "obj_loader.hpp"

#include <stdexcept>
#include <unordered_map>
#include <tuple>
#include <cstdint>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

ObjectLoader::ObjectLoader(const std::string& path)
{
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;          // ensure triangles
    config.vertex_color = false;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path, config)) {
        throw std::runtime_error("TinyObj: " + reader.Error());
    }
    if (!reader.Warning().empty()) {
        throw std::runtime_error("TinyObj warning: " + reader.Warning());
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    std::unordered_map<NormalTexturePoint, uint32_t, NormalTexturePointHasher> uniq;

    auto fetch3 = [&](const std::vector<float>& v, int idx)->glm::vec3 {
        if (idx < 0) return {};
        size_t i = static_cast<size_t>(3 * idx);
        return { v[i+0], v[i+1], v[i+2] };
    };
    auto fetch2 = [&](const std::vector<float>& v, int idx)->glm::vec2 {
        if (idx < 0) return {};
        size_t i = static_cast<size_t>(2 * idx);
        return { v[i+0], v[i+1] };
    };

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f]; // should be 3 (triangles)
            for (int vtx = 0; vtx < fv; ++vtx) {
                const tinyobj::index_t idx = shape.mesh.indices[index_offset + vtx];

                NormalTexturePoint v
                    (
                     fetch3(attrib.vertices,  idx.vertex_index),
                     fetch3(attrib.normals,  idx.normal_index),
                     fetch2(attrib.texcoords, idx.texcoord_index)
                    );

                auto it = uniq.find(v);
                if (it == uniq.end()) {
                    uint32_t newIndex = static_cast<uint32_t>(vertices_.size());
                    uniq.emplace(v, newIndex);
                    vertices_.push_back(v);
                    indices_.push_back(newIndex);
                } else {
                    indices_.push_back(it->second);
                }
            }
            index_offset += fv;
        }
    }
}
