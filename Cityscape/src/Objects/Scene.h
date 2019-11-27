// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Header.h
// Date: 21/11/2019

#pragma once

#include "Objects/Helicopter.h"
#include "Objects/PoliceCar.h"
#include "Objects/Water.h"

#include <midori/scene/Scene.h>
#include <midori/renderer/camera/Camera.h>

namespace City {

    class Scene {
    public:
        Scene();
        ~Scene() = default;

        void SetCamera(midori::Camera* camera) { m_CityScene.SetCamera(camera); }

        void SetScreenDimensions(uint32_t width, uint32_t height);

        void Draw();
        void CalculateShadows();
        void Update(float delta);

    private:
        midori::Scene m_CityScene;

        midori::ref<midori::LightingManager> m_LightingManager;
        midori::ref<midori::Shader> m_MeshLoadShader;

        Helicopter m_Helicopter;
        PoliceCar m_PoliceCar[5];
        Water m_Water[(4 * 2) + 4];

        // City Layout
        const float c_GroundSize = 75.0f;
        const float c_WaterTileSize = c_GroundSize * 0.5f;
        const uint32_t c_BuildingRows = 10;
        const uint32_t c_BuildingCols = 10;
        const float c_BuildingSize = 10.0f;
        const float c_StreetSize = 3.0f;

        inline midori::ref<midori::SceneObject> GetBaseBuilding();

        // Generation
        inline void AddGround();
        inline void AddBuildingGrid();
        inline void AddWater();
        inline void ConfWater(Water& waterTile, float posX, float posZ, uint32_t attachmentPoint = 0);

        // Lighting
        inline void AddLights();

        // Loading
        inline midori::ref<midori::SceneObject> LoadRandomBuilding();
        inline void LoadShaders();
        inline void LoadSkyBox();
    };

}
