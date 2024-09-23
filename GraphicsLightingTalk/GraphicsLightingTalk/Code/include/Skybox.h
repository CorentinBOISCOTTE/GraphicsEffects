#pragma once
#include "Math/matrice.h"
#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

#include "Shader.h"

class Skybox {
public:
    Skybox(std::vector<std::string> faces);

    unsigned int LoadCubemap(std::vector<std::string> faces);

    vector<std::string> faces
    {
        "Right.png",
        "Left.png",
        "Top.png",
        "Bottom.png",
        "Front.png",
        "Back.png"
    };
    unsigned int cubemapTexture = LoadCubemap(faces);

    void Draw(Shader& shader, const mat4x4& view, const mat4x4& projection);

    ~Skybox();

    unsigned int GetVAO() const { return VAO; }

private:
    unsigned int VAO, VBO;
};