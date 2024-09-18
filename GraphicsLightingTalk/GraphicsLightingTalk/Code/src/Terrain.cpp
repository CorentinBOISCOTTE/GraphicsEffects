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
    for (unsigned int i = 0; i < height; i += 3)
    {
        for (unsigned int j = 0; j < width; j += 3)
        {
            unsigned char* texel = data + (j + width * i) * channels;
            unsigned char y = texel[0];

            Vertex vertex1;
            Vertex vertex2;
            Vertex vertex3;
            vertex1.position.x = -height / 2.0f + i;
            vertex1.position.y = y * yScale - yShift;
            vertex1.position.z = -width / 2.0f + j;

            vertex2.position.x = -height / 2.0f + i;
            vertex2.position.y = y * yScale - yShift;
            vertex2.position.z = -width / 2.0f + j;

            vertex3.position.x = -height / 2.0f + i;
            vertex3.position.y = y * yScale - yShift;
            vertex3.position.z = -width / 2.0f + j;

            vertices.push_back(vertex1);
            vertices.push_back(vertex2);
            vertices.push_back(vertex3);
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
