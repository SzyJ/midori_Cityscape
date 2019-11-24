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
    }

}
