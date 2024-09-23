#pragma once
#include "Math/matrice.h"
#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

#include "Shader.h"

class Skybox : public IResource
{
public:
    Skybox();
    void Load() override;
    void Unload() override;
    bool IsLoaded() override;

    void LoadCubemap(std::vector<std::string> faces);

    unsigned int cubemapTexture = 0;

    void Draw(Shader* shader, const mat4x4& view, const mat4x4& projection);

    ~Skybox();

    unsigned int GetVAO() const { return VAO; }

private:
    unsigned int VAO, VBO;
};