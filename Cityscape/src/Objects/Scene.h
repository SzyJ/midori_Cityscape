// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Header.h
// Date: 21/11/2019

#pragma once

#include <midori/scene/Scene.h>
#include <midori/renderer/camera/Camera.h>

namespace City {

    class Scene {
    public:
        Scene();
        ~Scene() = default;

        void SetCamera(midori::Camera* camera) { m_CityScene.SetCamera(camera); }

        void SetScreenDientions(uint32_t width, uint32_t height) { m_LightingManager->UpdateFrameBufferSize(width, height); }

        void Draw();
        void Update(float delta);

    private:
        midori::Scene m_CityScene;
        midori::ref<midori::LightingManager> m_LightingManager;
        midori::ref<midori::Shader> m_MeshLoadShader;

        // City Layout
        const uint32_t c_BuildingRows = 10;
        const uint32_t c_BuildingCols = 10;
        const uint32_t c_BuildingSize = 10;
        const uint32_t c_StreetSize = 3;

        inline midori::ref<midori::SceneObject> GetBaseBuilding();

        // Generation
        inline void AddGround();
        inline void AddBuildingGrid();

        // Lighting
        inline void AddLights();

        // Loading
        inline midori::ref<midori::SceneObject> LoadRandomBuilding();
        inline void LoadShaders();
        inline void LoadSkyBox();
    };

}
