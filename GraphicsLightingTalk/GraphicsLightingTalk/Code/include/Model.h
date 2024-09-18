#pragma once
#include <vector>
#include <sstream>
#include <unordered_map>
#include "Math/matrice.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include "Buffer.h"
#include "VertexAttributes.h"
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"
#include "Material.h"
#include "Terrain.h"

enum class TYPE
{
    TRIANGLES,
    QUADS,
    TERRAIN,
    OTHER
};

class Model : public IResource
{
public:
    void Load() override;
    void LoadTerrain(Terrain terrain);
    void Unload() override;
    bool IsLoaded() override;
    void Draw(Camera* camera, Shader* shader, std::vector<Light*> lights, mat4x4 mvp, mat4x4 model, Texture* texture, Material* material);

private:
    void BindBuffers();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexBuffer;
    Buffer buffer;
    TYPE modelType;
    VertexAttributes attributes;
};