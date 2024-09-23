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
            
            vertex.normal = { 0, 1, 0 };

            vertices.push_back(vertex);
        }
    }

    /*std::vector<Vector3D> normals(vertices.size());*/

    for (unsigned int i = 0; i < height - 1; i++)
    {
        for (unsigned int j = 0; j < width - 1; j++)
        {
            //Indices
            unsigned int topLeft = i * width + j;
            unsigned int topRight = i * width + (j + 1);
            unsigned int bottomLeft = (i + 1) * width + j;
            unsigned int bottomRight = (i + 1) * width + (j + 1);

            indexBuffer.push_back(topLeft);
            indexBuffer.push_back(bottomLeft);
            indexBuffer.push_back(topRight);

            /*Vector3D v1 = vertices[topLeft].position;
            Vector3D v2 = vertices[bottomLeft].position;
            Vector3D v3 = vertices[topRight].position;

            Vector3D edge1 = v2 - v1;
            Vector3D edge2 = v3 - v1;
            Vector3D normal1 = edge1.multiplyVectors(edge2);
            normal1 = normal1.normalize(normal1);

            normals[topLeft] += normal1;
            normals[bottomLeft] += normal1;
            normals[topRight] += normal1;*/

            indexBuffer.push_back(topRight);
            indexBuffer.push_back(bottomLeft);
            indexBuffer.push_back(bottomRight);

            /*v1 = vertices[topRight].position;
            v2 = vertices[bottomLeft].position;
            v3 = vertices[bottomRight].position;

            edge1 = v2 - v1;
            edge2 = v3 - v1;
            Vector3D normal2 = edge1.multiplyVectors(edge2);
            normal2 = normal2.normalize(normal2);

            normals[topRight] += normal2;
            normals[bottomLeft] += normal2;
            normals[bottomRight] += normal2;*/
        }
    }

    /*for (unsigned int i = 0; i < vertices.size(); i++)
    {
        normals[i] = normals[i].normalize(normals[i]);
        vertices[i].normal = normals[i];
    }*/
    /*normals.clear();*/
    stbi_image_free(data);
}
