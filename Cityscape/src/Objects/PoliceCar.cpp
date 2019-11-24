// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: PoliceCar.cpp
// Date: 24/11/2019

#include "PoliceCar.h"
#include "Resources.h"

#include <midori/renderer/MeshLoader.h>

namespace City {

    PoliceCar::PoliceCar() {
        float carScale = 1.0f;

        m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

        m_Car = midori::make_ref<midori::SceneObject>();
        m_Car->SetPosition(m_Position);
        m_Car->SetScale(carScale);
        m_Car->SetMaterial(midori::Material::Silver());
        m_Car->SetVertexArray(midori::MeshLoader::Load(MODEL_POLICE_CAR));

        m_RedLight = midori::make_ref<midori::PointLight>();
        m_RedLight->Color = glm::vec3(1.0f, 0.0f, 0.0f);
        m_RedLight->Position = m_Position + glm::vec3(0.25f, 1.2f, 0.0f);

        m_BlueLight = midori::make_ref<midori::PointLight>();
        m_BlueLight->Color = glm::vec3(0.0f, 0.0f, 1.0f);
        m_BlueLight->Position = m_Position + glm::vec3(-0.25f, 1.2f, 0.0f);
    }

    void PoliceCar::Update(float delta) {
        m_TotalTime += delta * c_ChangeSpeed;

        if (glm::sin(m_TotalTime) > 0.0f) {
            m_RedLight->Color = glm::vec3(1.0f, 0.0f, 0.0f);
            m_BlueLight->Color = glm::vec3(0.0f, 0.0f, 0.0f);
        } else {
            m_RedLight->Color = glm::vec3(0.0f, 0.0f, 0.0f);
            m_BlueLight->Color = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        if (m_Position.x > m_SpawnPlane || m_Position.x < -m_SpawnPlane ||
            m_Position.z > m_SpawnPlane || m_Position.z < -m_SpawnPlane) {
            ChooseRandomRoad();
        }

        m_Position += m_MoveDirection * m_Speed * delta;
        m_Car->SetPosition(m_Position);
        m_BlueLight->Position = m_Position + glm::vec3(-0.25f, 1.2f, 0.0f);
        m_RedLight->Position = m_Position + glm::vec3(0.25f, 1.2f, 0.0f);
    }

    void PoliceCar::ChooseRandomRoad() {
        int rng = rand() % (m_RoadCount * 2 * 2);

        float offset = 0.5f;

        int roadIndex;
        if (rng >= m_RoadCount * 3) { // Left => Right
            roadIndex = rng - (m_RoadCount * 3);
            m_MoveDirection = glm::vec3(1.0f, 0.0f, 0.0f);

            float zCoord = -m_FurthestRoad + (static_cast<float>(roadIndex) * m_RoadSpacing);
            m_Position = glm::vec3(-m_SpawnPlane + offset, 0.0f, zCoord);

            m_Car->SetRotation(glm::vec3(0.0f, 0.5f, 0.0f));

        } else if (rng >= m_RoadCount * 2) { // Top => Bottom
            roadIndex = rng - (m_RoadCount * 2);
            m_MoveDirection = glm::vec3(0.0f, 0.0f, 1.0f);

            float xCoord = -m_FurthestRoad + (static_cast<float>(roadIndex) * m_RoadSpacing);
            m_Position = glm::vec3(xCoord, 0.0f, -m_SpawnPlane + offset);

            m_Car->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

        } else if (rng >= m_RoadCount) { // Right => Left
            roadIndex = rng - m_RoadCount;
            m_MoveDirection = glm::vec3(-1.0f, 0.0f, 0.0f);

            float zCoord = -m_FurthestRoad + (static_cast<float>(roadIndex) * m_RoadSpacing);
            m_Position = glm::vec3(m_SpawnPlane - offset, 0.0f, zCoord);

            m_Car->SetRotation(glm::vec3(0.0f, -0.5f, 0.0f));

        } else { // Bottom => Top
            roadIndex = rng;
            m_MoveDirection = glm::vec3(0.0f, 0.0f, -1.0f);

            float xCoord = -m_FurthestRoad + (static_cast<float>(roadIndex) * m_RoadSpacing);
            m_Position = glm::vec3(xCoord, 0.0f, m_SpawnPlane - offset);

            m_Car->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
        }

        m_Speed = 13.0f + static_cast<float>(rng % 4);

        m_TotalTime = 0;

    }

}
