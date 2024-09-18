#include "Terrain.h"
#include "stb_image.h"

void Terrain::Load(std::filesystem::path fileName)
{
    std::string outfilename_str = fileName.string();
    data = stbi_load(outfilename_str.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cout << "Erreur lors du chargement de la heightmap " << fileName << std::endl;
        return;
    }
    FillBuffers();
}

void Terrain::Unload()
{
    vertices.clear();
    indexBuffer.clear();
}

void Terrain::FillBuffers()
{
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            unsigned char* texel = data + (j + width * i) * channels;
            unsigned char y = texel[0];

            Vertex vertex;
            vertex.position.x = -height / 2.0f + i;
            vertex.position.y = y * yScale - yShift;
            vertex.position.z = -width / 2.0f + j;
            vertices.push_back(vertex);
        }
    }

    for (unsigned int i = 0; i < height - 1; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                indexBuffer.push_back(j + width * (i + k));
            }
        }
    }

    stbi_image_free(data);
}
