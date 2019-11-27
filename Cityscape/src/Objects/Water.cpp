// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Water.cpp
// Date: 26/11/2019

#include "Water.h"
#include "Resources.h"

#include <midori/renderer/RenderCommand.h>

#include <midori/scene/Skybox.h>

namespace City {

    Water::Water() {
        uint8_t shaders =
                (uint8_t) midori::ShaderProgramType::vertex |
                (uint8_t) midori::ShaderProgramType::fragment |
                (uint8_t) midori::ShaderProgramType::tessellation;
        m_Shader = midori::Shader::Load(SHADER_WATER, shaders);
        m_Shader->Bind();

        midori::RenderCommand::SetActiveTexture(0);
        m_WaterTex = midori::Texture2D::Create(TEXTURE_WATER_DISP);
        m_WaterTex->Bind(0);

        midori::RenderCommand::SetActiveTexture(1);
        m_WaterNormal = midori::Texture2D::Create(TEXTURE_WATER_NORMAL);
        m_WaterNormal->Bind(1);

        m_Shader->UploadUniformInt("u_WaterDisplacement", 0);
        m_Shader->UploadUniformInt("u_WaterNormal", 1);
        m_Shader->UploadUniformInt("u_CubeMapTexture", MD_CUBEMAP_TEXTURE_SLOT);
        m_Shader->Unbind();

        const int bufferSize = 4 * (3 + 2);
        float vertexBuffer[bufferSize]{
            -1.0f,  0.0f, -1.0f,    0.0f, 1.0f,
            -1.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             1.0f,  0.0f, -1.0f,    1.0f, 1.0f,
             1.0f,  0.0f,  1.0f,    1.0f, 0.0f
        };
        midori::ref<midori::VertexBuffer> groundVB = midori::VertexBuffer::Create(vertexBuffer, bufferSize);
        groundVB->SetLayout({
            {midori::ShaderDataType::Float3, "a_Position"},
            {midori::ShaderDataType::Float2, "a_TexCoord"}
            });

        auto groundVA = midori::VertexArray::Create();
        groundVA->AddVertexBuffer(groundVB);

        m_WaterPlane = midori::make_ref<midori::SceneObject>();
        m_WaterPlane->SetShader(m_Shader);
        m_WaterPlane->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));
        m_WaterPlane->SetVertexArray(groundVA);
        m_WaterPlane->SetScale(25.0f);
        m_WaterPlane->SetMaterial(midori::Material::Chrome());
        m_WaterPlane->SetGeometryPrimitive(midori::GeometryPrimitive::QuadPatches);
    }

}
