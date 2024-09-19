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
            unsigned char* texel1 = data + (j + width * i) * channels;
            unsigned char y1 = texel1[0];

            Vertex vertex1;
            vertex1.position.x = -height / 2.0f + i;
            vertex1.position.y = /*y1 * yScale - yShift*/0;
            vertex1.position.z = -width / 2.0f + j;
            
            vertices.push_back(vertex1);
        }
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        if (i < vertices.size() - 2)
        {
            Vector3D v1 = vertices[i + 1].position - vertices[i].position;
            Vector3D v2 = vertices[i + 2].position - vertices[i].position;
            vertices[i].normal = v1.multiplyVectors(v2) * -1;
        }
        else if (i < vertices.size() - 1)
        {
            Vector3D v1 = vertices[i + 1].position - vertices[i].position;
            Vector3D v2 = vertices[i - 1].position - vertices[i].position;
            vertices[i].normal = v1.multiplyVectors(v2) * -1;
        }
        else
        {
            Vector3D v1 = vertices[i - 2].position - vertices[i].position;
            Vector3D v2 = vertices[i - 1].position - vertices[i].position;
            vertices[i].normal = v1.multiplyVectors(v2) * -1;
        }
    }

    for (unsigned int i = 0; i < height - 1; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            for (unsigned int k = 0; k <= 2; k++)
            {
                indexBuffer.push_back(j + width * (i + k));
            }
        }
    }
    stbi_image_free(data);
}
