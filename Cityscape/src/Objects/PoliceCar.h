// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: PoliceCar.h
// Date: 24/11/2019

#pragma once

#include <midori/scene/SceneObject.h>
#include <midori/scene/lighting/Lights.h>

namespace City {

    class PoliceCar {
    public:
        PoliceCar();
        ~PoliceCar() = default;

        void SetShader(midori::ref<midori::Shader> shader) { m_Car->SetShader(shader); }

        void SetCityLayout(float furthestRoad, float roadSpacing, uint32_t roadCount, float spawnPlane) {
            m_FurthestRoad = furthestRoad;
            m_RoadSpacing = roadSpacing;
            m_RoadCount = roadCount;
            m_SpawnPlane = spawnPlane;
        }

        void ChooseRandomRoad();

        void Update(float delta);

        midori::ref<midori::SceneObject> GetModel() { return m_Car; }
        midori::ref<midori::PointLight> GetRedLight() { return m_RedLight; }
        midori::ref<midori::PointLight> GetBlueLight() { return m_BlueLight; }


    private:
        midori::ref<midori::SceneObject> m_Car;

        midori::ref<midori::PointLight> m_RedLight;
        midori::ref<midori::PointLight> m_BlueLight;

        glm::vec3 m_Position;
        glm::vec3 m_MoveDirection;

        float c_Speed = 15.0f;

        const float c_ChangeSpeed = 10.0f;
        float m_TotalTime = 0;

        float m_FurthestRoad;
        float m_RoadSpacing;
        uint32_t m_RoadCount;
        float m_SpawnPlane;
    };

}
