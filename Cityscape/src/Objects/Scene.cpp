// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Scene.cpp
// Date: 21/11/2019

#include "Scene.h"

#include "Resources.h"

#include <midori/renderer/MeshLoader.h>
#include <cstdlib>

namespace City {

    Scene::Scene() {
        AddLights();

        LoadShaders();
        LoadSkyBox();

        AddGround();
        AddWater();
        AddBuildingGrid();

        for (PoliceCar& car : m_PoliceCar) {
            car.SetShader(m_MeshLoadShader);
            float roadOffset = ((c_BuildingRows * 0.5f) - 1);
        
            car.SetCityLayout(roadOffset * c_BuildingSize + roadOffset * c_StreetSize, c_StreetSize + c_BuildingSize, c_BuildingRows, (roadOffset + 1) * c_BuildingSize + (roadOffset + 1) * c_StreetSize);
            car.ChooseRandomRoad();
            m_CityScene.AddOpaqueObject(car.GetModel());
            m_LightingManager->AddPointLight(car.GetRedLight());
            m_LightingManager->AddPointLight(car.GetBlueLight());
        }
        
        m_CityScene.AddOpaqueObject(m_Helicopter.GetSceneObject());
        m_LightingManager->AddSpotLight(m_Helicopter.GetSpotLight());
    }

    void Scene::SetScreenDimensions(uint32_t width, uint32_t height) {
        m_LightingManager->UpdateFrameBufferSize(width, height);
    }

    void Scene::CalculateShadows() {
        m_CityScene.DrawDepth();
    }

    void Scene::Draw() {
        midori::RenderCommand::SetClearColor({ 0.26f, 0.26f, 0.26f, 1.0f });
        midori::RenderCommand::Clear();

        m_CityScene.Draw();
    }


    void Scene::Update(float delta) {
        for (Water& waterTile : m_Water) {
            waterTile.Update(delta);
        }
        m_Helicopter.Update(delta);
        for (PoliceCar& car : m_PoliceCar) {
            car.Update(delta);
        }
    }

    void Scene::AddWater() {
        float maxDisplacement = c_GroundSize + c_WaterTileSize;
        float halfDisplacement = c_GroundSize * 0.5f;

        // Top Left
        ConfWater(m_Water[0], -maxDisplacement, -maxDisplacement);

        // Top
        ConfWater(m_Water[1], -halfDisplacement, -maxDisplacement, 4);
        ConfWater(m_Water[2],  halfDisplacement, -maxDisplacement, 4);

        // Top Right
        ConfWater(m_Water[3], maxDisplacement, -maxDisplacement);

        // Right
        ConfWater(m_Water[4], maxDisplacement, -halfDisplacement, 1);
        ConfWater(m_Water[5], maxDisplacement,  halfDisplacement, 1);

        // Bottom Right
        ConfWater(m_Water[6], maxDisplacement, maxDisplacement);

        // Bottom
        ConfWater(m_Water[7],  halfDisplacement, maxDisplacement, 2);
        ConfWater(m_Water[8], -halfDisplacement, maxDisplacement, 2);

        // Bottom Left
        ConfWater(m_Water[9], -maxDisplacement, maxDisplacement);

        // Left
        ConfWater(m_Water[10], -maxDisplacement,  halfDisplacement, 3);
        ConfWater(m_Water[11], -maxDisplacement, -halfDisplacement, 3);
    }

    void Scene::ConfWater(Water& waterTile, float posX, float posZ, uint32_t attachmentPoint) {
        waterTile.SetSize(c_WaterTileSize);
        waterTile.SetPosition(glm::vec3(posX, -4.5f, posZ));
        waterTile.SetAttachmentPoint(attachmentPoint);

        m_CityScene.AddOpaqueObject(waterTile.GetObject());
    }

    void Scene::AddGround() {
        unsigned int indexBuffer[6] = {
            0, 1, 2, 1, 2, 3
        };
        auto groundIB = midori::IndexBuffer::Create(indexBuffer, 6);

        const int bufferSize = 4 * (3 + 3 + 2);
        float vertexBuffer[bufferSize]{
            -1.0f,  0.0f, -1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
            -1.0f,  0.0f,  1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
             1.0f,  0.0f, -1.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
             1.0f,  0.0f,  1.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f
        };
        midori::ref<midori::VertexBuffer> groundVB = midori::VertexBuffer::Create(vertexBuffer, bufferSize);
        groundVB->SetLayout({
            {midori::ShaderDataType::Float3, "a_Position"},
            {midori::ShaderDataType::Float3, "a_Normal"},
            {midori::ShaderDataType::Float2, "a_TexCoord"}
            });

        auto groundVA = midori::VertexArray::Create();
        groundVA->AddVertexBuffer(groundVB);
        groundVA->SetIndexBuffer(groundIB);

        auto ground = midori::make_ref<midori::SceneObject>();
        ground->SetShader(m_MeshLoadShader);
        ground->SetVertexArray(groundVA);
        ground->SetScale(c_GroundSize);
        ground->SetMaterial(midori::Material::Chrome());

        m_CityScene.AddOpaqueObject(ground);
    }

    midori::ref<midori::SceneObject> Scene::GetBaseBuilding() {
        auto building = midori::make_ref<midori::SceneObject>();
        building->SetMaterial(midori::Material::Chrome());
        building->SetShader(m_MeshLoadShader);
        building->SetScale(5.0f);

        return building;
    }

    void Scene::AddBuildingGrid() {
        float gridRowCoordLimit = static_cast<float>(c_BuildingRows - 1) * 0.5f;
        float gridColCoordLimit = static_cast<float>(c_BuildingCols - 1) * 0.5f;

        const float padding = 0.05f;

        for (float zIndex = gridRowCoordLimit * -1; zIndex < gridRowCoordLimit + padding; zIndex += 1.0f) {
            for (float xIndex = gridColCoordLimit * -1; xIndex < gridColCoordLimit + padding; xIndex += 1.0f) {
                auto building = LoadRandomBuilding();
                float xCoord = (xIndex * c_BuildingSize) + (xIndex * c_StreetSize);
                float zCoord = (zIndex * c_BuildingSize) + (zIndex * c_StreetSize);

                building->SetPosition(glm::vec3(xCoord, 0.0f, zCoord));
                m_CityScene.AddOpaqueObject(building);
            }
        }

    }

    void Scene::AddLights() {
        m_LightingManager = midori::make_ref<midori::LightingManager>();

        m_LightingManager->AmbientStrength = 0.6f;

        auto blueDir = midori::make_ref<midori::DirectionalLight>(glm::vec3(0.5f, -0.74f, 0.47f), glm::vec3(0.19f, 0.23f, 0.5f));
        blueDir->Strength = 0.3f;
        blueDir->ShadowMap.OrthoNearZ = 5.0f;
        blueDir->ShadowMap.OrthoFarZ = 150.0f;
        blueDir->ShadowMap.OrthoDistFromOrigin = 75.0f;
        blueDir->ShadowMap.OrthoSize = 75.0f;
        m_LightingManager->AddDirectionalLight(blueDir);
        
        auto yellowDir = midori::make_ref<midori::DirectionalLight>(glm::vec3(-0.5f, -0.767f, 0.363f), glm::vec3(0.43f, 0.419f, 0.25f));
        yellowDir->Strength = 0.3f;
        yellowDir->ShadowMap.OrthoNearZ = 5.0f;
        yellowDir->ShadowMap.OrthoFarZ = 150.0f;
        yellowDir->ShadowMap.OrthoDistFromOrigin = 75.0f;
        yellowDir->ShadowMap.OrthoSize = 75.0f;
        m_LightingManager->AddDirectionalLight(yellowDir);

        auto moonDir = midori::make_ref<midori::DirectionalLight>(glm::vec3(0.0f, -1.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
        moonDir->Strength = 0.5f;
        moonDir->ShadowMap.OrthoNearZ = 5.0f;
        moonDir->ShadowMap.OrthoFarZ = 150.0f;
        moonDir->ShadowMap.OrthoDistFromOrigin = 75.0f;
        moonDir->ShadowMap.OrthoSize = 75.0f;
        m_LightingManager->AddDirectionalLight(moonDir);
        
        m_CityScene.SetLightManager(m_LightingManager);
    }

    midori::ref<midori::SceneObject> Scene::LoadRandomBuilding() {
        const uint8_t buildingTypes = 16;

        uint8_t buildingID = rand() % buildingTypes;

        std::string path(MODEL_BUILDING);
        path.append(std::to_string(buildingID + 1));
        path.append(".obj");

        auto buildingVA = midori::MeshLoader::Load(path);
        auto buildingObject = GetBaseBuilding();
        buildingObject->SetVertexArray(buildingVA);

        return buildingObject;
    }

    void Scene::LoadShaders() {
        m_MeshLoadShader = midori::Shader::Load(SHADER_MODEL_LOADER);
        m_MeshLoadShader->Bind();
    }

    void Scene::LoadSkyBox() {
        m_CityScene.SetSkybox(midori::make_ref<midori::Skybox>(TEXTURE_SKYBOX));
    }

}
