#include "Instancing.h"

Instancing::Instancing(std::filesystem::path filename, Shader* shader, Texture* texture, uint32_t nbInstances)
{
    this->shader = shader;
    this->nbInstances = nbInstances;
    this->texture = texture;

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
    CreatePositions();
    BindBuffers();
    position.clear();
    textureUV.clear();
    normal.clear();
    myfile.close();
}

Instancing::~Instancing()
{
    vertices.clear();
    indexBuffer.clear();
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Instancing::Draw(Camera* camera, mat4x4 model)
{
    shader->UseShader();
    texture->Bind();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_VBO);
    glBindVertexArray(attributes.m_VAO);
    glDrawElementsInstanced(GL_QUADS, indexBuffer.size(), GL_UNSIGNED_INT, 0, nbInstances);
    texture->Unbind();
}

void Instancing::CreatePositions()
{
    int offset = 10;
    for (int i = 0; i < nbInstances; i++)
    {
        instances.push_back({ 0.f, 30.f, (i * offset) - (nbInstances * 0.5f * offset), 1.f });
    }
}

void Instancing::BindBuffers()
{
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    attributes.Load();
    attributes.Bind();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), GL_STATIC_DRAW);

    attributes.Setup();
    attributes.Reset();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
