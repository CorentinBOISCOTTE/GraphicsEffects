#pragma once
#include "Math/matrice.h"
#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Shader.h"

class Skybox {
public:
    Skybox();
    ~Skybox();
    void Render(const mat4x4& projectionMatrix, const mat4x4& viewMatrix);

private:
    void InitSkybox();
    unsigned int cubemapTexture;
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    unsigned int skyboxShader;

    unsigned int LoadCubemap(std::vector<std::string> faces);
};
