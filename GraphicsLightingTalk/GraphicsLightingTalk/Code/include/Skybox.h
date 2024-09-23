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
    Skybox(std::vector<std::string> faces);

    void Draw(Shader& shader, const mat4x4& view, const mat4x4& projection);

    ~Skybox();

private:
    unsigned int VAO, VBO;
    unsigned int cubemapTexture;

    unsigned int LoadCubemap(std::vector<std::string> faces);
};