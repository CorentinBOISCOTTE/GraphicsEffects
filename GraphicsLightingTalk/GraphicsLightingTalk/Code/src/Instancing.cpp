#include "Instancing.h"

Instancing::Instancing(std::filesystem::path filename, Shader* shader)
{
    this->shader = shader;

    std::vector<Vector3D> position;
    std::vector<Vector2D> textureUV;
    std::vector<Vector3D> normal;
    textureUV.push_back(Vector2D(0.f, 0.f));
    int index = 0;
    std::map<std::tuple<int, int, int>, int> map;
    bool typeDefined = false;
    int nbVertices = 0.f;

    std::string line;

    std::ifstream myfile(filename);

    if (!myfile.is_open()) {
        std::cout << "Can't open the file, check your file path" << std::endl;
        return;
    }

    while (getline(myfile, line)) {
        Vertex vertex;
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vector3D pos;
            iss >> pos.x >> pos.y >> pos.z;
            position.push_back(pos);
        }
        else if (type == "vt") {
            Vector2D tex;
            iss >> tex.x >> tex.y;
            textureUV.push_back(tex);
        }
        else if (type == "vn") {
            Vector3D nor;
            iss >> nor.x >> nor.y >> nor.z;
            normal.push_back(nor);
        }
        else if (type == "f") {
            std::string faceData;
            while (iss >> faceData) {
                nbVertices++;
                std::istringstream faceStream(faceData);
                std::string vertexIndexStr, textureIndexStr, normalIndexStr;
                std::getline(faceStream, vertexIndexStr, '/');
                std::getline(faceStream, textureIndexStr, '/');
                std::getline(faceStream, normalIndexStr, '/');

                int vertexIndex = std::stoi(vertexIndexStr) - 1;
                int textureIndex = 0;
                if (!textureIndexStr.empty())
                    textureIndex = std::stoi(textureIndexStr);
                int normalIndex = 0;
                if (!normalIndexStr.empty())
                    normalIndex = std::stoi(normalIndexStr) - 1;

                Vertex vertex;
                vertex.position = position[vertexIndex];
                if (!textureIndexStr.empty())
                    vertex.textureUV = textureUV[textureIndex];
                else
                    vertex.textureUV = Vector2D(0.f, 0.f);
                if (!normalIndexStr.empty())
                    vertex.normal = normal[normalIndex];
                else
                    vertex.normal = Vector3D(0.f, 0.f, 0.f);

                std::tuple<int, int, int> vertexTuple = std::make_tuple(vertexIndex, textureIndex, normalIndex);
                auto it = map.find(vertexTuple);
                if (it != map.end()) {
                    indexBuffer.push_back(it->second);
                }
                else {
                    int newIndex = vertices.size();
                    vertices.push_back(vertex);
                    indexBuffer.push_back(newIndex);
                    map[vertexTuple] = newIndex;
                }
            }
        }
    }
    position.clear();
    textureUV.clear();
    normal.clear();
    myfile.close();
}

Instancing::~Instancing()
{
    vertices.clear();
    indexBuffer.clear();
}
