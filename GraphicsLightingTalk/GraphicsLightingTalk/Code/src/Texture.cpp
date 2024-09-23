#include "Texture.h"
#include "..\include\Texture.h"
#include "stb_image.h"

unsigned char* Texture::loadTexture(std::filesystem::path fileName, int& textureWidth, int& textureHeight) {
    std::string outfilename_str = fileName.string();
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureData = stbi_load(outfilename_str.c_str(), &textureWidth, &textureHeight, &channels, 0);
    if (!textureData) {
        std::cout << "Erreur lors du chargement de la texture " << fileName << std::endl;
        return nullptr;
    }

    return textureData;
}

void Texture::Load()
{
    /*data = loadTexture(filename, width, height);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    switch (channels)
    {
    case 1:
        glTextureStorage2D(texture, 0, GL_R8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_R8, GL_UNSIGNED_BYTE, data);
        break;
    case 2:
        glTextureStorage2D(texture, 0, GL_RG8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RG8, GL_UNSIGNED_BYTE, data);
        break;
    case 3:
        glTextureStorage2D(texture, 0, GL_RGB8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB8, GL_UNSIGNED_BYTE, data);
        break;
    case 4:
        glTextureStorage2D(texture, 0, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA8, GL_UNSIGNED_BYTE, data);
        break;
    }

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenerateTextureMipmap(texture);*/
    data = loadTexture(filename, width, height);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);


    switch (channels)
    {
    case 1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_R, GL_UNSIGNED_BYTE, data);
        break;
    case 2:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::LoadCubemap(const std::vector<std::string>& faces)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unload()
{
    glDeleteTextures(1, &texture);
    stbi_image_free(this->data);
}

bool Texture::IsLoaded()
{
	return false;
}
