#include "Instancing.h"

Instancing::Instancing(std::filesystem::path filename, uint32_t nbInstances)
{
    this->nbInstances = nbInstances;
    this->filename = filename;
}

Instancing::~Instancing()
{
    vertices.clear();
    indexBuffer.clear();
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VAO);
}

void Instancing::Load()
{
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
                    vertex.normal = Vector3D(0.f, 1.f, 0.f);

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
    vertices.clear();
}

void Instancing::Draw(Camera camera, mat4x4 model, Shader* shader, Texture* texture)
{
    mat4x4 MODEL = model;
    MODEL.GLMCompatible();

    mat4x4 vp = camera.getVP();
    vp.GLMCompatible();

    shader->UseShader();
    texture->Bind();
    shader->SetUniformMatrix4x4("model", MODEL);
    shader->SetUniformMatrix4x4("vp", vp);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_IBO);
    glBindVertexArray(m_VAO);
    glDrawElementsInstanced(GL_QUADS, indexBuffer.size(), GL_UNSIGNED_INT, nullptr, nbInstances);

    texture->Unbind();
}

void Instancing::CreatePositions()
{
    if (tempInstances.size() > 0)
    {
        if (nbInstances > tempInstances.size())
            nbInstances = tempInstances.size();
        auto gen = std::mt19937(1);
        std::sample(tempInstances.begin(), tempInstances.end(), std::back_inserter(instances), nbInstances, gen);
    }
    else
    {
        int offset = 10;
        for (int i = 0; i < nbInstances; i++)
        {
            instances.push_back({ 0.f, 30.f, (i * offset) - (nbInstances * 0.5f * offset), 1.f });
        }
    }
    tempInstances.clear();
}

void Instancing::BindBuffers()
{
    glCreateBuffers(1, &m_VBO);
    glCreateBuffers(1, &m_EBO);
    glCreateVertexArrays(1, &m_VAO);

    glNamedBufferStorage(m_VBO, vertices.size() * sizeof(Vertex), vertices.data(), 0);
    glNamedBufferStorage(m_EBO, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), 0);

    glEnableVertexArrayAttrib(m_VAO, 0);
    glEnableVertexArrayAttrib(m_VAO, 1);
    glEnableVertexArrayAttrib(m_VAO, 2);

    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, false, offsetof(Vertex, position));
    glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, false, offsetof(Vertex, normal));
    glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, false, offsetof(Vertex, textureUV));

    glVertexArrayAttribBinding(m_VAO, 0, 0);
    glVertexArrayAttribBinding(m_VAO, 1, 0);
    glVertexArrayAttribBinding(m_VAO, 2, 0);

    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_VAO, m_EBO);

    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, nbInstances * sizeof(Vector4D), instances.data(), 0);
}
