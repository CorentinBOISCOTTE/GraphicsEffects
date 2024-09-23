#include "Model.h"

void Model::Load() {
    std::vector<Vector3D> position;
    std::vector<Vector2D> textureUV;
    std::vector<Vector3D> normal;
    textureUV.push_back(Vector2D(0.f, 0.f));
    int index = 0; 
    std::map<std::tuple<int, int, int>, int> map;
    bool typeDefined = false;
    int nbVertices = 0.f;

    string line;

    ifstream myfile(filename);

    if (!myfile.is_open()) {
        std::cout << "Can't open the file, check your file path" << std::endl;
        return;
    }

    while (getline(myfile, line)) {
        Vertex vertex; 
        std::istringstream iss(line);
        string type;
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
            if (!typeDefined)
            {
                switch (nbVertices)
                {
                case 3:
                    modelType = TYPE::TRIANGLES;
                    break;
                case 4:
                    modelType = TYPE::QUADS;
                    break;
                default:
                    modelType = TYPE::OTHER;
                    break;
                }
                typeDefined = true;
            }
        }
    }
    BindBuffers();
    position.clear();
    textureUV.clear();
    normal.clear();
    myfile.close();
}

void Model::LoadTerrain(Terrain terrain)
{
    modelType = TYPE::TERRAIN;
    vertices = terrain.vertices;
    indexBuffer = terrain.indexBuffer;
    BindBuffers();
    terrain.Unload();
}

void Model::BindBuffers()
{
    buffer.Load();
    attributes.Load();
    attributes.Bind();

    buffer.Bind(vertices, indexBuffer);
    attributes.Setup();
    attributes.Reset();
    buffer.Reset();
}

void Model::Unload()
{
    vertices.clear();
    indexBuffer.clear();
}

bool Model::IsLoaded()
{
    if (vertices.size() > 0 && indexBuffer.size() > 0)
        return true;
    return false;
}

void Model::Draw(Camera* camera, Shader* shader, std::vector<Light*> lights, mat4x4 mvp, mat4x4 model, Texture* texture, Material* material)
{
    shader->UseShader();
    if (!IsLoaded())
    {
        std::cout << "Model is not loaded" << std::endl;
        return;
    }

    mat4x4 MVP = mvp;
    MVP.GLMCompatible();


    mat4x4 MODEL = model;
    MODEL.GLMCompatible();
    
    GLuint program = shader->GetProgram(); 
    texture->Bind();
    shader->SetUniformMatrix4x4("mvp", MVP);
    shader->SetUniformMatrix4x4("model", MODEL);

    mat4x4 normalMVP = model;
    normalMVP.getInverse();
    normalMVP.TransposeMatrix();


    shader->SetUniformMatrix4x4("normalMVP", normalMVP);

    shader->SetUniformVector3D("viewPos", camera->eye);

    shader->SetUniformColor("material.ambientColor", material->ambientColor);
    shader->SetUniformColor("material.diffuseColor", material->diffuseColor);
    shader->SetUniformColor("material.specularColor", material->specularColor);
    shader->SetUniformFloat("material.shininess", material->shininess);

    //Light
    int directionalLightCounter = 0;
    int pointLightCounter = 0;
    int spotLightCounter = 0;
    for (int i = 0; i < lights.size(); i++)
    {
        if (lights[i]->lightType == Light::LightType::Directional)
        {
            DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(lights[i]);

            shader->SetUniformColor("directionalLight.ambientColor", directionalLight->ambiantColor);
            shader->SetUniformColor("directionalLight.diffuseColor", directionalLight->diffuseColor);
            shader->SetUniformColor("directionalLight.specularColor", directionalLight->specularColor);
            shader->SetUniformVector3D("directionalLight.direction", directionalLight->direction);
            shader->SetUniformFloat("directionalLight.intensity", directionalLight->intensity);
            directionalLightCounter++;
        }
        else if (lights[i]->lightType == Light::LightType::Point)
        {
            PointLight* pointLight = dynamic_cast<PointLight*>(lights[i]);
            std::string ambiant = (std::ostringstream() << "pointLights[" << pointLightCounter << "].ambientColor").str();
            std::string diffuse = (std::ostringstream() << "pointLights[" << pointLightCounter << "].diffuseColor").str();
            std::string specular = (std::ostringstream() << "pointLights[" << pointLightCounter << "].specularColor").str();
            std::string position = (std::ostringstream() << "pointLights[" << pointLightCounter << "].position").str();

            std::string constant = (std::ostringstream() << "pointLights[" << pointLightCounter << "].constant").str();
            std::string linear = (std::ostringstream() << "pointLights[" << pointLightCounter << "].linear").str();
            std::string quadratic = (std::ostringstream() << "pointLights[" << pointLightCounter << "].quadratic").str();

            std::string intensity = (std::ostringstream() << "pointLights[" << pointLightCounter << "].intensity").str();

            shader->SetUniformColor(ambiant, pointLight->ambiantColor);
            shader->SetUniformColor(diffuse, pointLight->diffuseColor);
            shader->SetUniformColor(specular, pointLight->specularColor);
            shader->SetUniformVector3D(position, pointLight->position);
            shader->SetUniformColor(ambiant, pointLight->ambiantColor);
            shader->SetUniformFloat(constant, pointLight->constantValue);
            shader->SetUniformFloat(linear, pointLight->linearValue);
            shader->SetUniformFloat(quadratic, pointLight->quadraticValue);
            shader->SetUniformFloat(intensity, pointLight->intensity);
            pointLightCounter++;
        }
        else if (lights[i]->lightType == Light::LightType::Spot)
        {
            SpotLight* spotLight = dynamic_cast<SpotLight*>(lights[i]);
            std::string ambiant = (std::ostringstream() << "spotLights[" << spotLightCounter << "].ambientColor").str();
            std::string diffuse = (std::ostringstream() << "spotLights[" << spotLightCounter << "].diffuseColor").str();
            std::string specular = (std::ostringstream() << "spotLights[" << spotLightCounter << "].specularColor").str();
            std::string position = (std::ostringstream() << "spotLights[" << spotLightCounter << "].position").str();
            std::string direction = (std::ostringstream() << "spotLights[" << spotLightCounter << "].direction").str();

            std::string spotCosAngleName = (std::ostringstream() << "spotLights[" << spotLightCounter << "].spotCosAngle").str();
            std::string spotCosSmoothAngleName = (std::ostringstream() << "spotLights[" << spotLightCounter << "].spotCosSmoothAngle").str();

            std::string constant = (std::ostringstream() << "spotLights[" << spotLightCounter << "].constant").str();
            std::string linear = (std::ostringstream() << "spotLights[" << spotLightCounter << "].linear").str();
            std::string quadratic = (std::ostringstream() << "spotLights[" << spotLightCounter << "].quadratic").str();
            std::string intensity = (std::ostringstream() << "spotLights[" << spotLightCounter << "].intensity").str();

            shader->SetUniformColor(ambiant, spotLight->ambiantColor);
            shader->SetUniformColor(diffuse, spotLight->diffuseColor);
            shader->SetUniformColor(specular, spotLight->specularColor);
            shader->SetUniformVector3D(position, spotLight->position);
            shader->SetUniformVector3D(direction, spotLight->direction);

            float spotCosAngle = cosf(spotLight->spotAngle * M_PI / 180.0f);
            float spotCosSmoothValue = cosf((spotLight->spotAngle - spotLight->spotAngle * spotLight->spotSmoothValue) * M_PI / 180.0f);
            shader->SetUniformFloat(spotCosAngleName, spotCosAngle);
            shader->SetUniformFloat(spotCosSmoothAngleName, spotCosSmoothValue);

            shader->SetUniformFloat(constant, spotLight->constantValue);
            shader->SetUniformFloat(linear, spotLight->linearValue);
            shader->SetUniformFloat(quadratic, spotLight->quadraticValue);
            shader->SetUniformFloat(intensity, spotLight->intensity);

            spotLightCounter++;
        }
    }
    shader->SetUniformInt("pointLightCount", pointLightCounter);
    shader->SetUniformInt("spotLightCount", spotLightCounter);
    shader->SetUniformColor("globalAmbiantColor", LightingSettings::globalAmbiantColor);
    
    glBindVertexArray(attributes.m_VAO);
    switch (modelType)
    {
    case TYPE::TRIANGLES:
        glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
        break;
    case TYPE::QUADS:
        glDrawElements(GL_QUADS, indexBuffer.size(), GL_UNSIGNED_INT, 0);
        break;
    case TYPE::TERRAIN:
        glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
        break;
    case TYPE::OTHER:
        glDrawElements(GL_POLYGON, indexBuffer.size(), GL_UNSIGNED_INT, 0);
        break;
    default:
        glDrawElements(GL_POLYGON, indexBuffer.size(), GL_UNSIGNED_INT, 0);
        break;
    }
    glBindVertexArray(0);

    texture->Unbind();
}