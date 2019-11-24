// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Helicopter.cpp
// Date: 23/11/2019

#include "Helicopter.h"
#include "Resporces.h"

#include <midori/renderer/MeshLoader.h>
#include <midori/scene/Skybox.h>
#include <glm/gtx/perpendicular.hpp>

namespace City {

    Helicopter::Helicopter() {
        float helicopterScale = 2.0f;

        m_HelicopterShader = midori::Shader::Load(SHADER_HELICOPTER);
        m_HelicopterShader->Bind();
        m_HelicopterShader->UploadUniformInt("u_CubeMapTexture", MD_CUBEMAP_TEXTURE_SLOT);

        m_Chassis = midori::make_ref<midori::SceneObject>();
        m_Chassis->SetPosition(glm::vec3(0.0f, c_FlightHeight, 0.0f));
        m_Chassis->SetScale(helicopterScale);
        m_Chassis->SetShader(m_HelicopterShader);
        m_Chassis->SetMaterial(midori::Material::Silver());
        m_Chassis->SetVertexArray(midori::MeshLoader::Load(MODEL_HELICOPTER_CHASSIS));

        m_Propeller = midori::make_ref<midori::SceneObject>();
        m_Propeller->SetPosition(glm::vec3(0.0f, 0.54f, 0.0f));
        m_Propeller->SetShader(m_HelicopterShader);
        m_Propeller->SetMaterial(midori::Material::Silver());
        m_Propeller->SetVertexArray(midori::MeshLoader::Load(MODEL_HELICOPTER_PROPELLER));

        m_Chassis->AddChild(m_Propeller);
    }

    Helicopter::~Helicopter() { }

    void Helicopter::Update(float delta) {
        m_AngularPosition += c_FlightSpeed * delta;

        if (m_AngularPosition >= 2.0f * glm::pi<float>()) {
            m_AngularPosition -= 2.0f * glm::pi<float>();
        }

        glm::vec3 newPosition = glm::vec3(glm::sin(m_AngularPosition) * c_FlightArc, c_FlightHeight, glm::cos(m_AngularPosition) * c_FlightArc);


        float facingDir = m_AngularPosition / glm::pi<float>() - 1.0f;
        m_Chassis->SetPosition(newPosition);
        m_Chassis->SetRotation(glm::vec3(0.0f, facingDir - 0.5f, 0.0f));

        m_PropellerRotation += delta;
        m_PropellerRotation = glm::min(m_PropellerRotation, 2.0f);
        m_PropellerRotation -= 1.0f;

        m_Propeller->SetRotation(glm::vec3(0.0f, m_PropellerRotation, 0.0f));

    }


}
